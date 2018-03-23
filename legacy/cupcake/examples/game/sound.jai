
LoadLibrary :: __stdcall #no_context (name: *U8) -> *Void #foreign "LoadLibraryA"
GetProcAddress :: __stdcall #no_context (module: *Void, name: *U8) -> () -> Void #foreign

DirectSoundCreateType :: #type #no_context __stdcall (guid: *Void, object: **IDirectSound8, unknown: *Void) -> S32
DirectSoundCreate: DirectSoundCreateType

//

GUID :: struct @@pack(1) {  
    Data1: U32;
    Data2: U16;
    Data3: U16;
    Data4: [8] U8;
}

DSBUFFERCAPS :: struct @@type_size(8) @@pack(1) { }
DSBUFFERDESC :: struct @@pack(1) {
    dwSize        : S32;
    dwFlags       : S32;
    dwBufferBytes : S32;
    dwReserved    : S32;
    lpwfxFormat   : *Waveformatex;

    guid3DAlgorithm: GUID;
}

Waveformatex :: struct @@pack(1) {
    wFormatTag: S16
    nChannels: S16
    nSamplesPerSec: S32
    nAvgBytesPerSec: S32
    nBlockAlign: S16
    wBitsPerSample: S16
    cbSize: S16
}

IDirectSound8 :: struct @@pack(1) {
    using vtable: *IDirectSound8Vtable;
}

IDirectSoundBuffer8 :: struct @@pack(1)  {
    using vtable: *IDirectSoundBuffer8Vtable;
}

Com_Vtable_Base :: struct @@pack(1)  {
    QueryInterface: *Void;
    AddRef: *Void;
    Release: __stdcall #no_context (*Void) -> S32;
}

IDirectSoundBuffer8Vtable :: struct @@pack(1)  {
    using base: Com_Vtable_Base;

    GetCaps: __stdcall #no_context (buffer: *IDirectSoundBuffer8, caps: *DSBUFFERCAPS) ;
    GetCurrentPosition: __stdcall #no_context (buffer: *IDirectSoundBuffer8, pdwCurrentPlayCursor: *S32, pdwCurrentWriteCursor: *S32) -> S32 ;
    GetFormat: *Void;
    GetVolume: *Void;
    GetPan: *Void;
    GetFrequency: *Void;
    GetStatus:     __stdcall #no_context (buffer: *IDirectSoundBuffer8, pdwStatus: *S32) -> S32 ;
    Initialize: *Void;
    Lock: __stdcall #no_context (buffer: *IDirectSoundBuffer8, dwOffset: S32, dwBytes: S32, pvAudioPtr1: **Void, dwAudioBytes1: *S32, pvAudioPtr2: **Void, dwAudioBytes2: *S32, dwFlags: S32) -> S32 ;
    Play:     __stdcall #no_context (buffer: *IDirectSoundBuffer8, dwReserved1: S32, dwPriority: S32, dwFlags: S32) -> S32 ;
    SetCurrentPosition: *Void;
    SetFormat:  __stdcall #no_context (buffer: *IDirectSoundBuffer8, format: *Waveformatex) -> S32 ;
    SetVolume: *Void;
    SetPan: *Void;
    SetFrequency: *Void;
    Stop: __stdcall #no_context (buffer: *IDirectSoundBuffer8) -> S32 ;
    Unlock: __stdcall #no_context (buffer: *IDirectSoundBuffer8, pvAudioPtr1: *Void, dwAudioBytes1: S32, pvAudioPtr2: *Void, dwAudioBytes2: S32) -> S32 ;
    
    Restore: __stdcall #no_context (buffer: *IDirectSoundBuffer8) -> S32 ;

    // IDirectSoundBuffer8 methods
    SetFX: *Void;
    AcquireResources: *Void;
    GetObjectInPath: *Void;
}

IDirectSound8Vtable :: struct @@pack(1)  {
    using base: Com_Vtable_Base;

    // IDirectSound methods
    CreateSoundBuffer: __stdcall  #no_context (dsound: *IDirectSound8, desc: *DSBUFFERDESC, buffer: **IDirectSoundBuffer8, output: **Void) -> S32  ;
    
    GetCaps: *Void;
    DuplicateSoundBuffer: *Void;
    SetCooperativeLevel: __stdcall #no_context (dsound: *IDirectSound8, hwnd: *Void, level: S32) -> S32  ;
    Compact: *Void;
    GetSpeakerConfig: __stdcall (dsound: *IDirectSound8, config: *S32) -> S32 #no_context  ;
    SetSpeakerConfig: *Void;
    Initialize: *Void;
}

DSSPEAKER_HEADPHONE        :: 1;
DSSPEAKER_MONO             :: 2;
DSSPEAKER_QUAD             :: 3;
DSSPEAKER_STEREO           :: 4;
DSSPEAKER_SURROUND         :: 5;
DSSPEAKER_5POINT1          :: 6;
DSSPEAKER_5POINT1_BACK     :: DSSPEAKER_5POINT1;
DSSPEAKER_5POINT1_SURROUND :: 9;
DSSPEAKER_7POINT1          :: 7;
DSSPEAKER_7POINT1_SURROUND :: 8;
DSSPEAKER_7POINT1_WIDE     :: DSSPEAKER_7POINT1;

DSBCAPS_PRIMARYBUFFER      :: 0x1;
DSBCAPS_CTRLVOLUME         :: 0x80;
DSBCAPS_GLOBALFOCUS        :: 0x08000;
DSBCAPS_GETCURRENTPOSITION2:: 0x10000;

//

WindowsSuccess :: (result: S32) -> Bool {
    return result >= 0
}

SAMPLES_PER_SEC :: 48000

SoundID :: #type S64
SoundManager :: struct {
    dobject: *IDirectSound8
    dbuffer: *IDirectSoundBuffer8

    bytes_per_sample: S64
    buffer_size: S64
    current_sample_index: S64 = 0

    music_data: *Sound_Data
}

Sound_Data :: struct {
    channel_count: S64
    sample_count: S64
    samples: *U16

    file_data: [] U8
}

riff_code :: (format: String) -> U32 {
    assert(format.count == 4)

    value: U32
    value |= (cast(U32) format[0]) << 0
    value |= (cast(U32) format[1]) << 8
    value |= (cast(U32) format[2]) << 16
    value |= (cast(U32) format[3]) << 24

    return value
}

CHUNK_ID_RIFF :: #run riff_code("RIFF")
CHUNK_ID_WAVE :: #run riff_code("WAVE")
CHUNK_ID_fmt  :: #run riff_code("fmt ")
CHUNK_ID_data :: #run riff_code("data")

WAVE_Header :: struct @@pack(1) {
    riff_id: U32
    size: U32
    wave_id: U32
}

WAVE_FORMAT_FILE_PCM :: 0x01

WAVE_fmt :: struct @@pack(1) {
    format_tag: U16
    channel_count: U16
    samples_per_sec: U32
    avg_bytes_per_sec: U32
    block_align: U16
    bits_per_sample: U16
    cbSize: U16
    valid_bits_per_sample: U16
    channel_mask: U32
    sub_format: [16] U8
}

WAVE_Chunk :: struct @@pack(1) {
    id: U32
    size: U32
}

riff_iterator :: struct {
    at: *U8
    stop: *U8
}

parse_chunk :: (from: *U8, stop: *U8) -> riff_iterator {
    riff: riff_iterator
    riff.at = from
    riff.stop = stop

    return riff
}

next_chunk :: (it: riff_iterator) -> riff_iterator {
    chunk := cast(*WAVE_Chunk) it.at
    size := (chunk.size + 1) & ~1

    it.at += size_of(WAVE_Chunk) + chunk.size
    return it
}

is_chunk_valid :: (it: riff_iterator) -> Bool {
    return it.at < it.stop
}

get_chunk_data :: (it: riff_iterator) -> *Void {
    return it.at + size_of(WAVE_Chunk)
}


// NOTE(Wase): WAV file loader
load_sound :: (manager: *SoundManager, filename: String) -> *Sound_Data {

    stream := fopen(to_c_string(filename), to_c_string("rb"))
    if !stream then return null

    fseek(stream, 0, SEEK_END)
    size := ftell(stream)
    fseek(stream, 0, SEEK_SET)

    if size <= 0 then return null
    file_buffer := new [size] U8 ---

    read_size := fread(file_buffer.data, 1, cast(S32) file_buffer.count, stream)
    if read_size <= 0 then return null
    fclose(stream);

    data := file_buffer.data
    wave_header := cast(*WAVE_Header) data
    if wave_header.riff_id != CHUNK_ID_RIFF || wave_header.wave_id != CHUNK_ID_WAVE {
        print("Error: '%' unsupported wav file\n", filename)
        return null
    }

    sample_data: *Void
    sample_size: S64
    channel_count: S64

    start :: cast(*U8) (wave_header + 1)
    stop :: start + wave_header.size - 4
    it := parse_chunk(start, stop)
    while is_chunk_valid(it) {
        chunk := cast(*WAVE_Chunk) it.at

        if (chunk.id == CHUNK_ID_fmt) {
            wave_fmt := cast(*WAVE_fmt) get_chunk_data(it)

            assert(wave_fmt.format_tag == WAVE_FORMAT_FILE_PCM)
            assert(wave_fmt.samples_per_sec == SAMPLES_PER_SEC)
            assert(wave_fmt.bits_per_sample == (manager.bytes_per_sample * 8) / 2)
            assert(wave_fmt.block_align == (2 * wave_fmt.channel_count))
            
            channel_count = wave_fmt.channel_count
        } else if (chunk.id == CHUNK_ID_data) {
            wave_data := get_chunk_data(it)
            sample_data = wave_data
            sample_size = chunk.size
        }

        it = next_chunk(it)
    } 

    sound := new Sound_Data
    sound.file_data = file_buffer;
    sound.channel_count = channel_count
    sound.sample_count = sample_size / (2 * size_of(U16))

    assert(sound.channel_count >= 1 && sound.channel_count <= 2)
    sound.samples = cast(*U16) sample_data
    return sound
}

init_direct_sound :: (handle: *Void) -> *SoundManager {
    dsound_library := LoadLibrary(to_c_string("dsound.dll"))
    if !dsound_library {
        print("Error: DirectSound not found!\n")
        return null
    }

    dobject: *IDirectSound8
    DirectSoundCreate = cast(DirectSoundCreateType) GetProcAddress(dsound_library, to_c_string("DirectSoundCreate"))
    if !DirectSoundCreate {
        print("Error: No 'DirectSoundCreate' function found.\n")
        return null
    }

    result := DirectSoundCreate(null, *dobject, null)
    if !WindowsSuccess(result) {
        print("Error: Could not create directsound object.\n")
        return null
    }

    DSSCL_NORMAL        :: 0x1
    DSSCL_PRIORITY      :: 0x2
    DSSCL_EXCLUSIVE     :: 0x3
    DSSCL_WRITEPRIMARY  :: 0x4
    result = dobject.SetCooperativeLevel(dobject, handle, DSSCL_PRIORITY)
    if !WindowsSuccess(result) {
        print("Error: Could not set cooperative level.\n")
        return null
    }

    WAVE_FORMAT_PCM :: 0x0001

    waveformat: Waveformatex
    waveformat.wFormatTag = WAVE_FORMAT_PCM
    waveformat.nChannels = 2
    waveformat.nSamplesPerSec = SAMPLES_PER_SEC
    waveformat.wBitsPerSample = 16
    waveformat.nBlockAlign = (waveformat.nChannels * waveformat.wBitsPerSample) / 8
    waveformat.nAvgBytesPerSec = (waveformat.nSamplesPerSec * waveformat.nBlockAlign)
    waveformat.cbSize = cast(S16) size_of(Waveformatex)

    primary_buffer_desc: DSBUFFERDESC
    primary_buffer_desc.dwSize = cast(S32) size_of(DSBUFFERDESC)
    primary_buffer_desc.dwFlags = DSBCAPS_PRIMARYBUFFER

    primary: *IDirectSoundBuffer8
    result = dobject.CreateSoundBuffer(dobject, *primary_buffer_desc, *primary, 0)
    if !WindowsSuccess(result) {
        print("Error: Could not create primary buffer.\n")
        return null
    }

    result = primary.SetFormat(primary, *waveformat);
    if !WindowsSuccess(result) {
        print("Error: Could not set format for primary buffer.\n")
        return null
    }

    secondary_buffer_desc: DSBUFFERDESC
    secondary_buffer_desc.dwSize = cast(S32) size_of(DSBUFFERDESC)
    secondary_buffer_desc.dwFlags = 0
    secondary_buffer_desc.dwBufferBytes = SAMPLES_PER_SEC * 2 * cast(S32) size_of(S32)
    secondary_buffer_desc.lpwfxFormat = *waveformat

    secondary: *IDirectSoundBuffer8
    result = dobject.CreateSoundBuffer(dobject, *secondary_buffer_desc, *secondary, 0)
    if !WindowsSuccess(result) {
        print("Error: Could not create secondary buffer.\n")
        return null
    }

    DSBPLAY_LOOPING :: 0x00000001
    result = secondary.Play(secondary, 0, 0, DSBPLAY_LOOPING);
    if !WindowsSuccess(result) {
        print("Error: Could not play secondary buffer.\n")
        return null
    }

    sound := new SoundManager
    sound.dobject = dobject
    sound.dbuffer = secondary
    sound.bytes_per_sample = 4
    sound.buffer_size = SAMPLES_PER_SEC * 2 * size_of(S32)

    return sound
}



swp := SAMPLES_PER_SEC / 256 // NOTE(Wase): Middle C
hswp := swp / 2

sound_update :: (using manager: *SoundManager) {
    play_offset: S32 = 0
    write_offset: S32 = 0
    bytes := 0

    buffer1: [] U16
    buffer2: [] U16

    result := dbuffer.GetCurrentPosition(dbuffer, *play_offset, *write_offset);
    if !WindowsSuccess(result) {
        print("Error: Could not get current position.\n")
        return ;
    }

    bytes_to_lock := (current_sample_index * bytes_per_sample) % buffer_size
    if bytes_to_lock > play_offset {
        bytes = buffer_size - bytes_to_lock
        bytes += play_offset
    } else {
        bytes = play_offset - bytes_to_lock
    }

    if bytes == 0 return;

    bytes_1_count: S32
    bytes_2_count: S32
    result = dbuffer.Lock(dbuffer, cast(S32) bytes_to_lock, cast(S32) bytes, cast(**Void) *buffer1.data, *bytes_1_count, cast(**Void) *buffer2.data, *bytes_2_count, 0)

    if !WindowsSuccess(result) {
        print("Error: Could not lock sound buffer.\n")
        return;
    }

    buffer1.count = bytes_1_count / bytes_per_sample
    buffer2.count = bytes_2_count / bytes_per_sample

    volume := 0.1
    for sample_index: 0..buffer1.count-1 {
        left: S32 = 0
        right: S32 = 0

        index := current_sample_index
        left += cast(S16) music_data.samples[2 * index + 0]
        right += cast(S16) music_data.samples[2 * index + 1]


        uleft := (left * volume)
        uright := (right * volume)
        buffer1[2 * sample_index + 0] = cast(U16) uleft
        buffer1[2 * sample_index + 1] = cast(U16) uright
        current_sample_index += 1
    }

    for sample_index: 0..buffer2.count-1 {
        left: S32 = 0
        right: S32 = 0

        index := current_sample_index
        left += cast(S16) music_data.samples[2 * index + 0]
        right += cast(S16) music_data.samples[2 * index + 1]

        uleft := (left * volume)
        uright := (right * volume)
        buffer2[2 * sample_index + 0] = cast(U16) uleft
        buffer2[2 * sample_index + 1] = cast(U16) uright
        current_sample_index += 1
    }

    result = dbuffer.Unlock(dbuffer, buffer1.data, bytes_1_count, buffer2.data, bytes_2_count)
    if !WindowsSuccess(result) {
        print("Error: Could not unlock sound buffer.\n")
        return ;
    }
}
