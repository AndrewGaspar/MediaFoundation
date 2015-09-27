using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading.Tasks;
using MediaFoundationNative;
using Windows.Foundation;
using Windows.Storage;
using Windows.Storage.Streams;

namespace MediaFoundation
{
    public sealed class SourceReader : IDisposable
    {
        private SourceReaderNative Native;

        private SourceReader(SourceReaderNative native)
        {
            Native = native;
        }

        public static SourceReader CreateFromStream(IRandomAccessStreamWithContentType stream)
        {
            return new SourceReader(SourceReaderNative.CreateFromByteStream(stream));
        }

        public static IAsyncOperation<SourceReader> CreateFromUriAsync(Uri uri)
        {
            return AsyncInfo.Run(async token =>
            {
                if (uri.IsFile)
                {
                    var file = await StorageFile.GetFileFromPathAsync(uri.ToString());

                    var stream = await file.OpenReadAsync();

                    return CreateFromStream(stream);
                }
                else
                {
                    return new SourceReader(SourceReaderNative.CreateFromUrl(uri.ToString()));
                }
            });
        }

        public TimeSpan GetDurationBlocking()
        {
            return Native.GetDuration();
        }

        public void Dispose()
        {
            Native.Dispose();
        }
    }
}