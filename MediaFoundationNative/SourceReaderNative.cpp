#include "pch.h"
#include "SourceReaderNative.h"

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")

using namespace MediaFoundationNative;
using namespace Platform;
using namespace Microsoft::WRL;

#define THROW_IF_FAILED(hr)     { auto _hr = hr; if (FAILED(_hr)) throw Platform::Exception::CreateException(_hr); }
#pragma comment(lib, "mfreadwrite.lib")

SourceReaderNative::~SourceReaderNative()
{
    auto hr = MFShutdown();

    if (FAILED(hr))
    {
        __fastfail(FAST_FAIL_FATAL_APP_EXIT);
    }
}

SourceReaderNative^
SourceReaderNative::CreateFromUrl(Platform::String^ url)
{
    THROW_IF_FAILED(MFStartup(MF_VERSION));

    ComPtr<IMFSourceReader> sourceReader;
    try
    {
        THROW_IF_FAILED(MFCreateSourceReaderFromURL(url->Data(), nullptr, &sourceReader));
    }
    catch (Platform::Exception^)
    {
        THROW_IF_FAILED(MFShutdown());
        throw;
    }

    return ref new SourceReaderNative(std::move(sourceReader));
}

SourceReaderNative^
SourceReaderNative::CreateFromByteStream(Windows::Storage::Streams::IRandomAccessStreamWithContentType^ stream)
{
    THROW_IF_FAILED(MFStartup(MF_VERSION));

    ComPtr<IMFByteStream> byteStream;
    ComPtr<IMFAttributes> attributes;
    ComPtr<IMFSourceReader> sourceReader;
    try
    {
        THROW_IF_FAILED(MFCreateMFByteStreamOnStreamEx(reinterpret_cast<IUnknown*>(stream), &byteStream));

        THROW_IF_FAILED(byteStream.As(&attributes));
        THROW_IF_FAILED(attributes->SetString(MF_BYTESTREAM_CONTENT_TYPE, stream->ContentType->Data()));

        THROW_IF_FAILED(MFCreateSourceReaderFromByteStream(byteStream.Get(), nullptr, &sourceReader));
    }
    catch (Platform::Exception^)
    {
        THROW_IF_FAILED(MFShutdown());
        throw;
    }

    return ref new SourceReaderNative(std::move(sourceReader));
}

Windows::Foundation::TimeSpan
SourceReaderNative::GetDuration()
{
    PROPVARIANT prop;
    THROW_IF_FAILED(sourceReader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &prop));

    Windows::Foundation::TimeSpan timeSpan;
    timeSpan.Duration = static_cast<long long>(prop.uhVal.QuadPart);

    return timeSpan;
}