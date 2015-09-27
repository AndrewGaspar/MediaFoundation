#pragma once

#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <wrl.h>

namespace MediaFoundationNative
{
    public ref class SourceReaderNative sealed
    {
        Microsoft::WRL::ComPtr<IMFSourceReader> sourceReader;

        SourceReaderNative(Microsoft::WRL::ComPtr<IMFSourceReader>&& reader) : sourceReader(std::move(reader)) {}

    public:
        virtual ~SourceReaderNative();

        static SourceReaderNative^ CreateFromUrl(Platform::String^ url);
        static SourceReaderNative^ CreateFromByteStream(Windows::Storage::Streams::IRandomAccessStreamWithContentType^ stream);

        Windows::Foundation::TimeSpan GetDuration();
    };
}
