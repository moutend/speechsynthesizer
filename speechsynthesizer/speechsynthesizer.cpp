#include "stdafx.h"
#include <roapi.h>
#include <ppltasks.h>
#include <windows.media.speechsynthesis.h>
#include <windows.storage.streams.h>
#include <robuffer.h>
#pragma comment(lib, "runtimeobject.lib")

using namespace Platform;
using namespace Concurrency;
using namespace Windows::Foundation::Collections;
using namespace Windows::Media::SpeechSynthesis;
using namespace Windows::Storage::Streams;

extern "C" __declspec(dllexport) HRESULT TextToStream(
	UINT32 voiceNumber,
	String^ text,
	IBuffer^* buffer
) {
	auto ss = ref new SpeechSynthesizer();
	auto vs = ss->AllVoices;

	if (voiceNumber > 0 && voiceNumber <= vs->Size) {
		ss->Voice = vs->GetAt(voiceNumber - 1);
	}
	task<SpeechSynthesisStream^> t = create_task(
		ss->SynthesizeTextToStreamAsync(text)
	);
	try {
		t.wait();
		t.then([buffer](SpeechSynthesisStream^ stream) {
			auto dr = ref new DataReader(stream->GetInputStreamAt(0));
			create_task(
				dr->LoadAsync(stream->Size)
			).wait();
			*buffer = dr->ReadBuffer(stream->Size);
		});
	}
	catch (const Exception^ e) {
		return 1;
	}
	return 0;
}

extern "C" __declspec(dllexport) HRESULT SsmlToStream(
	UINT32 voiceNumber,
	String^ ssml,
	IBuffer^* buffer
) {
	auto ss = ref new SpeechSynthesizer();
	auto vs = ss->AllVoices;

	if (voiceNumber > 0 && voiceNumber <= vs->Size) {
		ss->Voice = vs->GetAt(voiceNumber - 1);
	}
	task<SpeechSynthesisStream^> t = create_task(
		ss->SynthesizeSsmlToStreamAsync(ssml)
	);
	try {
		t.wait();
		t.then([buffer](SpeechSynthesisStream^ stream) {
			auto dr = ref new DataReader(stream->GetInputStreamAt(0));
			create_task(
				dr->LoadAsync(stream->Size)
			).wait();
			*buffer = dr->ReadBuffer(stream->Size);
		});
	}
	catch (const Exception^ e) {
		return 1;
	}
	return 0;
}

extern "C" __declspec(dllexport) HRESULT GetVoices(
	IVectorView<VoiceInformation^>^* voices
) {
	auto ss = ref new SpeechSynthesizer();
	*voices = ss->AllVoices;
	return 0;
}