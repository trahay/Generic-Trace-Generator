<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="page">
    <name>index</name>
    <title>Open Trace Format 2</title>
    <filename>index</filename>
    <docanchor file="index">introduction</docanchor>
    <docanchor file="index">get_started</docanchor>
  </compound>
  <compound kind="file">
    <name>otf2.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>otf2_8h</filename>
    <includes id="OTF2__Reader_8h" name="OTF2_Reader.h" local="no" imported="no">otf2/OTF2_Reader.h</includes>
  </compound>
  <compound kind="file">
    <name>OTF2_Archive.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__Archive_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__Callbacks_8h" name="OTF2_Callbacks.h" local="no" imported="no">otf2/OTF2_Callbacks.h</includes>
    <includes id="OTF2__DefWriter_8h" name="OTF2_DefWriter.h" local="no" imported="no">otf2/OTF2_DefWriter.h</includes>
    <includes id="OTF2__DefReader_8h" name="OTF2_DefReader.h" local="no" imported="no">otf2/OTF2_DefReader.h</includes>
    <includes id="OTF2__EvtWriter_8h" name="OTF2_EvtWriter.h" local="no" imported="no">otf2/OTF2_EvtWriter.h</includes>
    <includes id="OTF2__EvtReader_8h" name="OTF2_EvtReader.h" local="no" imported="no">otf2/OTF2_EvtReader.h</includes>
    <includes id="OTF2__SnapWriter_8h" name="OTF2_SnapWriter.h" local="no" imported="no">otf2/OTF2_SnapWriter.h</includes>
    <includes id="OTF2__SnapReader_8h" name="OTF2_SnapReader.h" local="no" imported="no">otf2/OTF2_SnapReader.h</includes>
    <includes id="OTF2__GlobalDefWriter_8h" name="OTF2_GlobalDefWriter.h" local="no" imported="no">otf2/OTF2_GlobalDefWriter.h</includes>
    <includes id="OTF2__GlobalDefReader_8h" name="OTF2_GlobalDefReader.h" local="no" imported="no">otf2/OTF2_GlobalDefReader.h</includes>
    <includes id="OTF2__GlobalEvtReader_8h" name="OTF2_GlobalEvtReader.h" local="no" imported="no">otf2/OTF2_GlobalEvtReader.h</includes>
    <includes id="OTF2__GlobalSnapReader_8h" name="OTF2_GlobalSnapReader.h" local="no" imported="no">otf2/OTF2_GlobalSnapReader.h</includes>
    <includes id="OTF2__Thumbnail_8h" name="OTF2_Thumbnail.h" local="no" imported="no">otf2/OTF2_Thumbnail.h</includes>
    <includes id="OTF2__MarkerWriter_8h" name="OTF2_MarkerWriter.h" local="no" imported="no">otf2/OTF2_MarkerWriter.h</includes>
    <includes id="OTF2__MarkerReader_8h" name="OTF2_MarkerReader.h" local="no" imported="no">otf2/OTF2_MarkerReader.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_CHUNK_SIZE_DEFINITIONS_DEFAULT</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>af7d0601bb6ab8d725cb461a70a78467f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_CHUNK_SIZE_EVENTS_DEFAULT</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>aa407390c03c62a6c26321ea2d94e8fef</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_Archive_struct</type>
      <name>OTF2_Archive</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a52cbb2ef5f547a0b933ae596e592d694</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_MasterSlaveMode</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a23f3cb566c5ca416b2d825054bcf6ed3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_MasterSlaveMode_enum</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a03ce733174ea882d3e45bb45992b566c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SLAVE</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a03ce733174ea882d3e45bb45992b566ca9b6f5ab1ea6227a6f52e57e4061c62b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MASTER</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a03ce733174ea882d3e45bb45992b566ca09d26d204ac74fc4f3e001927f6183d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_Close</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a326853c89451869a30f51455e7833434</anchor>
      <arglist>(OTF2_Archive *archive)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_CloseDefReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a7d976990e580eb71925fadbe57270e89</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_DefReader *reader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_CloseDefWriter</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>ab027913c17aa9a3b45756b7d81e53aa2</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_DefWriter *writer)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_CloseEvtReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>aa3802ce154de4d71a672f1cab3b7ec09</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_EvtReader *reader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_CloseEvtWriter</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>affb10676969b31fa3c8cbe7bf5b255f3</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_EvtWriter *writer)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_CloseGlobalDefReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>afe47f1d19b9e8d8d0317bbd9e562b7c5</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_GlobalDefReader *globalDefReader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_CloseGlobalEvtReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a2f45ba8138738a3cff321c6e5b904793</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_GlobalEvtReader *globalEvtReader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_CloseGlobalSnapReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a7d7ba746b477aade234719adaa927aa6</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_GlobalSnapReader *globalSnapReader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_CloseMarkerReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a30baef927e34cfebf47f3832cd3abb05</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_MarkerReader *markerReader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_CloseMarkerWriter</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a9c7af67000c4c7fa8cf3db4c6a5e0090</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_MarkerWriter *writer)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_CloseSnapReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>ab5cd97cfebd301e003483a1ade4e600c</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_SnapReader *reader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_CloseSnapWriter</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a101b6d4a2c0bbcc2e4fc39e7dfee2940</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_SnapWriter *writer)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_CloseThumbReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a40f34a4e5446699923dfaf0692a2d7d8</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_ThumbReader *reader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetChunkSize</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a74dec7401ac5ea840dea041f7e8629c2</anchor>
      <arglist>(OTF2_Archive *archive, uint64_t *chunkSizeEvents, uint64_t *chunkSizeDefs)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetCompression</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>aadf3a6642140b13472839badad981705</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_Compression *compression)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetCreator</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a261e4c6bd760acebc28757bd27cd16de</anchor>
      <arglist>(OTF2_Archive *archive, char **creator)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_DefReader *</type>
      <name>OTF2_Archive_GetDefReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a189088054cc280023b71404969942c6b</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_LocationRef location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_DefWriter *</type>
      <name>OTF2_Archive_GetDefWriter</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a8af1a31ec94968e0adce079a42b865d1</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_LocationRef location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetDescription</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a10ceb13965496a619c8a19a913262511</anchor>
      <arglist>(OTF2_Archive *archive, char **description)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_EvtReader *</type>
      <name>OTF2_Archive_GetEvtReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a5f94ae592a151f691938dd350bfe3557</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_LocationRef location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_EvtWriter *</type>
      <name>OTF2_Archive_GetEvtWriter</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a8978e56e335c85313d4dc81e53d469b1</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_LocationRef location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetFileSubstrate</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a345fb7e5c19e34549266cd1081949e5b</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_FileSubstrate *substrate)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_GlobalDefReader *</type>
      <name>OTF2_Archive_GetGlobalDefReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a51cdf35f621889c0f5139d161a031a03</anchor>
      <arglist>(OTF2_Archive *archive)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_GlobalDefWriter *</type>
      <name>OTF2_Archive_GetGlobalDefWriter</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a169c3b62e0c2181009e1f90a7e09756b</anchor>
      <arglist>(OTF2_Archive *archive)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_GlobalEvtReader *</type>
      <name>OTF2_Archive_GetGlobalEvtReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a7f72c9e4e09131a9b36a417f309fff89</anchor>
      <arglist>(OTF2_Archive *archive)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_GlobalSnapReader *</type>
      <name>OTF2_Archive_GetGlobalSnapReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a77374a0874bcd0485fe2d3d74f39fd4b</anchor>
      <arglist>(OTF2_Archive *archive)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetMachineName</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a3ef9a09c9b5ae6caa44b7eeda38057e3</anchor>
      <arglist>(OTF2_Archive *archive, char **machineName)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_MarkerReader *</type>
      <name>OTF2_Archive_GetMarkerReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>ace820300231a9058cba44be6e03d373a</anchor>
      <arglist>(OTF2_Archive *archive)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_MarkerWriter *</type>
      <name>OTF2_Archive_GetMarkerWriter</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>ae2e2a65cff8ee213c66b80492f5528b1</anchor>
      <arglist>(OTF2_Archive *archive)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetMasterSlaveMode</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>af86ac6d46ee40c23b077501e4c21cd5c</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_MasterSlaveMode *masterOrSlave)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetNumberOfGlobalDefinitions</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>aac6f302bae7f96f25c5864cf0d96dd52</anchor>
      <arglist>(OTF2_Archive *archive, uint64_t *numberOfDefinitions)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetNumberOfLocations</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a513f1c652ab60154aa6dbb45ad1a014e</anchor>
      <arglist>(OTF2_Archive *archive, uint64_t *numberOfLocations)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetNumberOfSnapshots</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a4e99922800246f042b58484449f825cb</anchor>
      <arglist>(OTF2_Archive *archive, uint32_t *number)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetNumberOfThumbnails</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a7433a2339d06a04ba32b8eaf416ed117</anchor>
      <arglist>(OTF2_Archive *archive, uint32_t *number)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetProperty</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>ab4fa4ce68fad4bbd839d51117ce4d048</anchor>
      <arglist>(OTF2_Archive *archive, const char *name, char **value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetPropertyNames</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a070e9f249df5e30646652431bd544e52</anchor>
      <arglist>(OTF2_Archive *archive, uint32_t *numberOfProperties, char ***names)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_SnapReader *</type>
      <name>OTF2_Archive_GetSnapReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a284ced2253bb9d37aa9a4253bda209da</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_LocationRef location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_SnapWriter *</type>
      <name>OTF2_Archive_GetSnapWriter</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a96fd96de0cd99f8374a9f75b3a82a163</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_LocationRef location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ThumbReader *</type>
      <name>OTF2_Archive_GetThumbReader</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>ad3666944aeeab7601455062a9b096beb</anchor>
      <arglist>(OTF2_Archive *archive, uint32_t number)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ThumbWriter *</type>
      <name>OTF2_Archive_GetThumbWriter</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a79a9f9536c8581128b6ae7b08cf9b5ed</anchor>
      <arglist>(OTF2_Archive *archive, const char *name, const char *description, OTF2_ThumbnailType type, uint32_t numberOfSamples, uint32_t numberOfMetrics, const uint64_t *refsToDefs)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetTraceId</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a1631f681a1ce6a61af515722ee78cf82</anchor>
      <arglist>(OTF2_Archive *archive, uint64_t *id)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_GetVersion</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a13d201c6fc2e67d7596f4b903ff7d93f</anchor>
      <arglist>(OTF2_Archive *archive, uint8_t *major, uint8_t *minor, uint8_t *bugfix)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_Archive *</type>
      <name>OTF2_Archive_Open</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a90bb7ed515c59acc4e69446e81678991</anchor>
      <arglist>(const char *archivePath, const char *archiveName, const OTF2_FileMode fileMode, const uint64_t chunkSizeEvents, const uint64_t chunkSizeDefs, const OTF2_FileSubstrate fileSubstrate, const OTF2_Compression compression)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_SetBoolProperty</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>af63ad6095defcc923aa7c19525583a88</anchor>
      <arglist>(OTF2_Archive *archive, const char *name, bool value, bool overwrite)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_SetCreator</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>ab98dcf0bb4e78ac4cd4d431eca016b73</anchor>
      <arglist>(OTF2_Archive *archive, const char *creator)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_SetDescription</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>af1dfdc362b9c8f76c53bc4a1c80c2e1a</anchor>
      <arglist>(OTF2_Archive *archive, const char *description)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_SetFileSionCallbacks</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a0611c5d9128bbf3980d3f4b4e7913002</anchor>
      <arglist>(OTF2_Archive *archive, const OTF2_FileSionCallbacks *fileSionCallbacks, void *fileSionData)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_SetFlushCallbacks</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a97e285c61ec33f100774d3e8d0b46dc1</anchor>
      <arglist>(OTF2_Archive *archive, const OTF2_FlushCallbacks *flushCallbacks, void *flushData)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_SetMachineName</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>ae06d386387632a57c4a526aad6e3ce42</anchor>
      <arglist>(OTF2_Archive *archive, const char *machineName)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_SetMasterSlaveMode</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>afd60b1901b165b7bccc17844f5f40d5e</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_MasterSlaveMode masterOrSlave)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_SetMemoryCallbacks</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a825d81c4597624709769eb8b2fb04de7</anchor>
      <arglist>(OTF2_Archive *archive, const OTF2_MemoryCallbacks *memoryCallbacks, void *memoryData)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_SetNumberOfSnapshots</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>ab649b2c335f3bd175b6ff9cfbe2a0cda</anchor>
      <arglist>(OTF2_Archive *archive, uint32_t number)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_SetProperty</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a49dbb0c06c62a86545f1ee7d3652535b</anchor>
      <arglist>(OTF2_Archive *archive, const char *name, const char *value, bool overwrite)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Archive_SwitchFileMode</name>
      <anchorfile>OTF2__Archive_8h.html</anchorfile>
      <anchor>a3f47b92376d708afa37396dfc7b725aa</anchor>
      <arglist>(OTF2_Archive *archive, OTF2_FileMode newFileMode)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_AttributeList.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__AttributeList_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__GeneralDefinitions_8h" name="OTF2_GeneralDefinitions.h" local="no" imported="no">otf2/OTF2_GeneralDefinitions.h</includes>
    <class kind="union">OTF2_AttributeValue</class>
    <member kind="typedef">
      <type>struct OTF2_AttributeList_struct</type>
      <name>OTF2_AttributeList</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a8c6ef0aad1dded1c7cede6d5d2256325</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddAttribute</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a3b75adedaa7bc0bf1d286fb434d7f345</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_Type type, OTF2_AttributeValue attributeValue)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddAttributeRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a5394101dd1d4fd8c1c06952232ad2064</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_AttributeRef attributeRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddCommRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a2e0ac94b7f7ac13c36685711566081bd</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_CommRef commRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddDouble</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>af9867c36a86166f2416cae24af6bbb21</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, double float64Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddFloat</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a763149af563ef76c98bf6bcb5432357c</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, float float32Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddGroupRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a32548d04106c127b74b052f63c696124</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_GroupRef groupRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddInt16</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a5d6fbb98e6780718e3c6b314d1176c8f</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, int16_t int16Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddInt32</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a36d9e586729c12936454902e7b1c1012</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, int32_t int32Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddInt64</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>ad0e114a3c449b3e6d7fa66c4a37518bb</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, int64_t int64Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddInt8</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>aec2f75809d3b69849eb9b14d73ad86cb</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, int8_t int8Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddLocationRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a00076020288c70e78f21bb9721ea4653</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_LocationRef locationRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddMetricRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a3fabd3ac915ab338e62801087d523a5c</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_MetricRef metricRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddParameterRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>aec77b72b2503996904b88823f1e3a271</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_ParameterRef parameterRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddRegionRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a289ac08854b7d3eab3ede3c5a95b20fe</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_RegionRef regionRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddRmaWinRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>ad4ec9887601a904917f49205717e7cb3</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_RmaWinRef rmaWinRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddString</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a0e8d32b841bbce47268884cb6a51a4a2</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_StringRef stringRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddStringRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a8117d91272bf6faf3753fbd04b6cf8cc</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_StringRef stringRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddUint16</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>aa94d4cabfb6c4025f52947ae8b6fae51</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, uint16_t uint16Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddUint32</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a445d6a32a51658d22cbf3d5c53a34e5b</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, uint32_t uint32Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddUint64</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a09f003e7150f07f92466689c1ba31ee3</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, uint64_t uint64Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_AddUint8</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a9e63546ee39b86b9f089775fb8743569</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, uint8_t uint8Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_Delete</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>ac4c3ac2c5923b87b29df049408123d4f</anchor>
      <arglist>(OTF2_AttributeList *attributeList)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetAttributeByID</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>adba548e55891df4c1e160572c2cf54e4</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_Type *type, OTF2_AttributeValue *attributeValue)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetAttributeByIndex</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a6195cdbca14ec1dbe9cea5f269df19b4</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, uint32_t index, OTF2_AttributeRef *attribute, OTF2_Type *type, OTF2_AttributeValue *attributeValue)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetAttributeRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>ae486188c01b6cdae99e83df4e93a77d5</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_AttributeRef *attributeRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetCommRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a2321d53f011e650c31fb26c2c2f5a590</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_CommRef *commRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetDouble</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a8125edd04afda949cc2875433740078d</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, double *float64Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetFloat</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a9a8084b30c06af55bb18c030861b5e73</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, float *float32Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetGroupRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>aaedb5dbccf91a0e6942b771e72553820</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_GroupRef *groupRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetInt16</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>affd77f50afcc0f60486b8611a11dfd64</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, int16_t *int16Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetInt32</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a5aa816b7b7a6ff9f45059991ae7affe1</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, int32_t *int32Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetInt64</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>ab788821c741e8973b2499efa3f19c35f</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, int64_t *int64Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetInt8</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>aca4dbe6bee81f66fa94186aa879d10f7</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, int8_t *int8Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetLocationRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>ad3d6a4f7b74217c63094206b7828285f</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_LocationRef *locationRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetMetricRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a0df4d6b7c12ed095668907b78cf13763</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_MetricRef *metricRef)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>OTF2_AttributeList_GetNumberOfElements</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a153d3a0d6c7b22ad96036bc9306dc164</anchor>
      <arglist>(const OTF2_AttributeList *attributeList)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetParameterRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>aaa398cf2245b67fb641151832df694e7</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_ParameterRef *parameterRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetRegionRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>aae7993441ce6a1c28cb493b6b0876429</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_RegionRef *regionRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetRmaWinRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>aa32f7d30d0436bfe3ac5679db98a3a58</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_RmaWinRef *rmaWinRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetString</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a60f5191044782ba941b1b2e0c9df43cf</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_StringRef *stringRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetStringRef</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a3357340543245aed294671954fd2b2c3</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, OTF2_StringRef *stringRef)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetUint16</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a230013d5d9c0e133dbf32757d13e9dcf</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, uint16_t *uint16Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetUint32</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>ad4feada4db9cce8afebc559c8cb9861c</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, uint32_t *uint32Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetUint64</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a05e530fa1236cf70d5d488a011b32dd7</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, uint64_t *uint64Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_GetUint8</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>ab8e4404081ad731be52633e532b79134</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute, uint8_t *uint8Value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_AttributeList *</type>
      <name>OTF2_AttributeList_New</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a87c63770f4e5dee9e6883c4a4e21dbe5</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_PopAttribute</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>ababfd9b5b37759cbbf09993a0c8bef01</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef *attribute, OTF2_Type *type, OTF2_AttributeValue *attributeValue)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_RemoveAllAttributes</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>abe41b4280e7d4fb2b7fe966fa5dbd366</anchor>
      <arglist>(OTF2_AttributeList *attributeList)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_AttributeList_RemoveAttribute</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>ae54c9139c6b58457fc6e994a2c1a6db0</anchor>
      <arglist>(OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>OTF2_AttributeList_TestAttributeByID</name>
      <anchorfile>OTF2__AttributeList_8h.html</anchorfile>
      <anchor>a2273016aba7c0e85570dbc1270fddb75</anchor>
      <arglist>(const OTF2_AttributeList *attributeList, OTF2_AttributeRef attribute)</arglist>
    </member>
    <docanchor file="OTF2__AttributeList_8h">attribute_usage_write</docanchor>
  </compound>
  <compound kind="file">
    <name>OTF2_Callbacks.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__Callbacks_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__GeneralDefinitions_8h" name="OTF2_GeneralDefinitions.h" local="no" imported="no">otf2/OTF2_GeneralDefinitions.h</includes>
    <class kind="struct">OTF2_FileSionCallbacks</class>
    <class kind="struct">OTF2_FlushCallbacks</class>
    <class kind="struct">OTF2_MemoryCallbacks</class>
    <member kind="typedef">
      <type>int(*</type>
      <name>OTF2_FileSionClose</name>
      <anchorfile>OTF2__Callbacks_8h.html</anchorfile>
      <anchor>af22d96e47ea4ab5fd3cc83158d50da86</anchor>
      <arglist>)(void *userData, OTF2_FileType fileType, OTF2_LocationRef location, int sid)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_ErrorCode(*</type>
      <name>OTF2_FileSionGetRank</name>
      <anchorfile>OTF2__Callbacks_8h.html</anchorfile>
      <anchor>aba389fbdbe7858ef9f559c4b478e7a5e</anchor>
      <arglist>)(void *userData, OTF2_FileType fileType, OTF2_LocationRef location, int32_t *rank)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>OTF2_FileSionOpen</name>
      <anchorfile>OTF2__Callbacks_8h.html</anchorfile>
      <anchor>a32d8c131d3779286b24f4ab1a8301d4f</anchor>
      <arglist>)(void *userData, OTF2_FileType fileType, OTF2_LocationRef location, const char *fname, const char *fileMode, long long int *chunkSize, int *fsblkSize, FILE **filePtr)</arglist>
    </member>
    <member kind="typedef">
      <type>void *(*</type>
      <name>OTF2_MemoryAllocate</name>
      <anchorfile>OTF2__Callbacks_8h.html</anchorfile>
      <anchor>a2486102cd13d16782ff4f12d1a13ce33</anchor>
      <arglist>)(void *userData, OTF2_FileType fileType, OTF2_LocationRef location, void **perBufferData, uint64_t chunkSize)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>OTF2_MemoryFreeAll</name>
      <anchorfile>OTF2__Callbacks_8h.html</anchorfile>
      <anchor>ad7937b88490ee6af49898a473274b631</anchor>
      <arglist>)(void *userData, OTF2_FileType fileType, OTF2_LocationRef location, void **perBufferData, bool final)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_TimeStamp(*</type>
      <name>OTF2_PostFlushCallback</name>
      <anchorfile>OTF2__Callbacks_8h.html</anchorfile>
      <anchor>ae3ea780b646ee69e182516fb3daf756c</anchor>
      <arglist>)(void *userData, OTF2_FileType fileType, OTF2_LocationRef location)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_FlushType(*</type>
      <name>OTF2_PreFlushCallback</name>
      <anchorfile>OTF2__Callbacks_8h.html</anchorfile>
      <anchor>a3a10eed5627c067c01a85067c3ec0823</anchor>
      <arglist>)(void *userData, OTF2_FileType fileType, OTF2_LocationRef location, void *callerData, bool final)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_Definitions.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__Definitions_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__GeneralDefinitions_8h" name="OTF2_GeneralDefinitions.h" local="no" imported="no">otf2/OTF2_GeneralDefinitions.h</includes>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_GroupFlag</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a7c46150900229c7835047fe5591a83df</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_GroupType</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>afe07fb5d4d28c399103cb464f259c328</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_LocationGroupType</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a4bb6bcd1950a6e441aa51610afd07e30</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_LocationType</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad035905ca0af3abe4eb44f817ae7e058</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_MetricBase</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>abdd028be42e5aa526aeda5ee3eb96cd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_MetricMode</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a9e178238d6d7d16263334417a549ee7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_MetricOccurrence</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a87301a2cde0b8b953bd867ab80f0aeb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_MetricScope</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a3cd217d23a390173223dafa25fe216a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_MetricTiming</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>abf8c6b2e917bce4e7920c134692f397d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_MetricType</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>afba3f6166ae11042a0916cd6000e3fc7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_MetricValueProperty</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a93d8bdc9e03ede8e30b45fad4460235f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_ParameterType</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a1377af3bfa7c40a9e7796c3fa8e4db5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_RecorderKind</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a836583c0e4d7139ab6db8bd16a1dad36</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_RegionFlag</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a93215ce4072412a7af2c1d473d14b901</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_RegionRole</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa40a588fa7a48297e8236fb31fde4de3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_SystemTreeDomain</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a3d39e08949007f730e68bebad7b4c5f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_GroupFlag_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a338e0c0370beb0be7e2d4defca5b6e0d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_GROUP_FLAG_NONE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a338e0c0370beb0be7e2d4defca5b6e0da25080b57df32939b0bfe29b9ac8737aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_GROUP_FLAG_GLOBAL_MEMBERS</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a338e0c0370beb0be7e2d4defca5b6e0da6af268ed8b9c238086113d50f03767c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_GroupType_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a7741303dc83407956bf83d050fe692d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_GROUP_TYPE_UNKNOWN</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a7741303dc83407956bf83d050fe692d1a74c01bafc0f01511a7511c373ce406c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_GROUP_TYPE_LOCATIONS</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a7741303dc83407956bf83d050fe692d1a3fad0845f377d3b06a1d8256230d4921</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_GROUP_TYPE_REGIONS</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a7741303dc83407956bf83d050fe692d1ae98c90a230c2dc0e908039bb4855987d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_GROUP_TYPE_METRIC</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a7741303dc83407956bf83d050fe692d1ad9ed706cd872d36b701bb297642fe303</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_GROUP_TYPE_COMM_LOCATIONS</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a7741303dc83407956bf83d050fe692d1a7c0743c0cebb7e551a9af1927a56c42d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_GROUP_TYPE_COMM_GROUP</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a7741303dc83407956bf83d050fe692d1ab88ad8568d403f68c7f2b31257de5c3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_GROUP_TYPE_COMM_SELF</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a7741303dc83407956bf83d050fe692d1ab0fd0707c3376d1f7093124b7f868e12</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_LocationGroupType_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a8d990c41b07e51b1c32c196061a05958</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_LOCATION_GROUP_TYPE_UNKNOWN</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a8d990c41b07e51b1c32c196061a05958a7955d5a8c322d79c640c3ff8dd04ed68</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_LOCATION_GROUP_TYPE_PROCESS</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a8d990c41b07e51b1c32c196061a05958aba96a07f1d3c4af4626b0a52a6395a8c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_LocationType_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a23720faa2077e3e16f017e7de2cd4f56</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_LOCATION_TYPE_UNKNOWN</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a23720faa2077e3e16f017e7de2cd4f56a6a04fb783dbd90ae395d800a965297d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_LOCATION_TYPE_CPU_THREAD</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a23720faa2077e3e16f017e7de2cd4f56af38d206a03ccd28046bc73879ea60ad1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_LOCATION_TYPE_GPU</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a23720faa2077e3e16f017e7de2cd4f56aca56394bc8da586e7935901d360d228c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_LOCATION_TYPE_METRIC</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a23720faa2077e3e16f017e7de2cd4f56a9440bf08243f70e9d1652ede1515e992</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_MetricBase_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a2f2bc5056b49d3304092c21f535f774d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_BASE_BINARY</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a2f2bc5056b49d3304092c21f535f774da2535f1571a56997a569b64e168c47950</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_BASE_DECIMAL</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a2f2bc5056b49d3304092c21f535f774da2301357bc1a09a169387776d5b556d84</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_MetricMode_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa8321e67c497616d9fae3496dd406bc3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_ACCUMULATED_START</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa8321e67c497616d9fae3496dd406bc3a2d898d555ed9fd1c75a51e706eff0e4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_ACCUMULATED_POINT</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa8321e67c497616d9fae3496dd406bc3a6927a6263529d7df174eb291be3e3e22</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_ACCUMULATED_LAST</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa8321e67c497616d9fae3496dd406bc3a59ef663139f29e06ed2e18dd8e4f62c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_ACCUMULATED_NEXT</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa8321e67c497616d9fae3496dd406bc3a23e5336b44faaa33896d422e5d50bcac</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_ABSOLUTE_POINT</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa8321e67c497616d9fae3496dd406bc3a8cf8fc2a69424cba87fd9ae86a015f46</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_ABSOLUTE_LAST</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa8321e67c497616d9fae3496dd406bc3aefb5dcb94969c8082643f24ea4e947fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_ABSOLUTE_NEXT</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa8321e67c497616d9fae3496dd406bc3a49a0b50c3ba934d3fe17ca9ef6422622</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_RELATIVE_POINT</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa8321e67c497616d9fae3496dd406bc3a9d0a1efae49bdaa2eb226b16721b6718</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_RELATIVE_LAST</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa8321e67c497616d9fae3496dd406bc3ae5b2e75d960dd6d7ded7cd67cd06133a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_RELATIVE_NEXT</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa8321e67c497616d9fae3496dd406bc3abcfda6a67a314335300bbe4ee593f1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_MetricOccurrence_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a2dccdca001fe68bd1a3335bb8487ccce</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_SYNCHRONOUS_STRICT</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a2dccdca001fe68bd1a3335bb8487ccceab8f92e7081e4436bdf0a399596ce2a2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_SYNCHRONOUS</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a2dccdca001fe68bd1a3335bb8487ccceab4db458ddf1923c879d4442799f58b83</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_ASYNCHRONOUS</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a2dccdca001fe68bd1a3335bb8487cccea8c17c2e839aaf271980fdf15ce4a2baf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_MetricScope_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a57002c82b86435716c2e56fbee761288</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SCOPE_LOCATION</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a57002c82b86435716c2e56fbee761288a6c7af0d8fab54052bf73b089272435f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SCOPE_LOCATION_GROUP</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a57002c82b86435716c2e56fbee761288aecd274dc97d1689fc73656d98d24fdef</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SCOPE_SYSTEM_TREE_NODE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a57002c82b86435716c2e56fbee761288af7aae519461feb102e8786e2f8b2c810</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SCOPE_GROUP</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a57002c82b86435716c2e56fbee761288a0dbb69277166ec5fffed5bbc4cf14b26</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_MetricTiming_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a573dc3bd6c52d7ac4a846a30724fd44b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_TIMING_START</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a573dc3bd6c52d7ac4a846a30724fd44bae76419b710c2de401672cb8061054fd8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_TIMING_POINT</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a573dc3bd6c52d7ac4a846a30724fd44baec47c46bfe36515e5a2647f905baa918</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_TIMING_LAST</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a573dc3bd6c52d7ac4a846a30724fd44baf9a7f2def6f382dd0925403fef60d09a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_TIMING_NEXT</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a573dc3bd6c52d7ac4a846a30724fd44bab8c6c610ad01ae5db97000b3d69c2174</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_TIMING_MASK</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a573dc3bd6c52d7ac4a846a30724fd44ba03b1074712756cf2012ddc471fb90c24</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_MetricType_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa621c395b55f8ce23353c38912e806d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_TYPE_OTHER</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa621c395b55f8ce23353c38912e806d1a3dc5258d085059f120759a46d18c0ff5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_TYPE_PAPI</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa621c395b55f8ce23353c38912e806d1ad12fd85dce654a29a9c3ca0622c2f9a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_TYPE_RUSAGE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa621c395b55f8ce23353c38912e806d1a9d81e2c9f491a75fbade8e481a4a1ee6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_TYPE_USER</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>aa621c395b55f8ce23353c38912e806d1a6cf3c650db5d8b8bdef8a951be6132df</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_MetricValueProperty_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a602650303e72e947780965a9fc4ce340</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_VALUE_ACCUMULATED</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a602650303e72e947780965a9fc4ce340a7fc9a6c16ce02aa85b570f359bb39d48</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_VALUE_ABSOLUTE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a602650303e72e947780965a9fc4ce340aa6216888762a9bd94ada4376ba692e61</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_VALUE_RELATIVE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a602650303e72e947780965a9fc4ce340a6b8efa96cf0bac75b2c87291b19657b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_METRIC_VALUE_MASK</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a602650303e72e947780965a9fc4ce340afc0f50988e5aa6b6d24fdaf2512dfd55</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_ParameterType_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a36c052101a173ee19d747aec72e79fad</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_PARAMETER_TYPE_STRING</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a36c052101a173ee19d747aec72e79fadae2c1b96e66ef4acc61ee0951c8722cbe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_PARAMETER_TYPE_INT64</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a36c052101a173ee19d747aec72e79fada407148bc49a10e375bd3dbc9c9ec73c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_PARAMETER_TYPE_UINT64</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>a36c052101a173ee19d747aec72e79fada942f81b6aaf053e3822a0e5dda0c2bea</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_RecorderKind_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ac9f6c2ec682b698698c0ea4cbef599bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_RECORDER_KIND_UNKNOWN</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ac9f6c2ec682b698698c0ea4cbef599bda8276d0cc56903bb803b7f1b63123adc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_RECORDER_KIND_ABSTRACT</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ac9f6c2ec682b698698c0ea4cbef599bda051ed8b95726155c69f49f363787c710</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_RECORDER_KIND_CPU</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ac9f6c2ec682b698698c0ea4cbef599bda61fa04082e70d5065f11fdd53f4373cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_RECORDER_KIND_GPU</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ac9f6c2ec682b698698c0ea4cbef599bdaa6eb4fe833fd99b8146d1b3a456961e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_RegionFlag_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ae05312235cef40cabe7414b491ef35be</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_FLAG_NONE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ae05312235cef40cabe7414b491ef35bea84a657b4ffeb36fbb2abc7dc1e8e0615</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_FLAG_DYNAMIC</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ae05312235cef40cabe7414b491ef35beab3e863491221159e1639de57606a58ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_FLAG_PHASE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ae05312235cef40cabe7414b491ef35beaccf1103b6e138fb20bec745236a574ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_RegionRole_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_UNKNOWN</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096ac906b07211818d140cd9aaa35ae55c6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_FUNCTION</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a8e4cade87061dbed55563ad09332980d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_WRAPPER</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a3b667754c53273f5a05a724f5ea7c477</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_LOOP</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096ad82a313358d3d4eed054979da1645c18</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_CODE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096ade93d78260c98aa39ddaae8de29c949c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_PARALLEL</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a201b6a714af2b65bbe1243a8f40e91dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_SECTIONS</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a0af8bbd948ea9d26ad4af48929f7bf0d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_SECTION</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096ac8a12b1aff9a8ec9934d7d22114c7a7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_WORKSHARE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096ac3eb734f907bbfa4c3c0347557d9d630</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_SINGLE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096afbe414ccab718fbea7daa29a922188b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_SINGLE_SBLOCK</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a2c16295f21561a9ce5c22a2273113ca2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_MASTER</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a50c6ea1152921cf1309eca08033f6de5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_CRITICAL</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a52f56409fdb425b09985b2fc7eadfdf5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_CRITICAL_SBLOCK</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096ae5df806d42c558abf77a3c76a132b103</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_ATOMIC</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a4d90e4258045e1b239669e647e943720</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_BARRIER</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a59f802fa4a7a29e6d395b5e6f265ae3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_IMPLICIT_BARRIER</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a9eb393b83c709cb066d44f4eafdc2c62</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_FLUSH</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096ab17c577c0968e96e2044392ae01a7668</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_ORDERED</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a0cda647a8b49ca61a6cf8758f5983eef</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_ORDERED_SBLOCK</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a7f90e1424eb3207e36b29d3a6907f2a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_TASK</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a7f14ade6a12435520e2e2b6f28d4c981</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_TASK_CREATE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a7270fa5cdba147957912c7ec6722666b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_TASK_WAIT</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a218b546cb650b27d284b06767674cfc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_COLL_ONE2ALL</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a6ff4209156d7150e984e78f703ed2766</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_COLL_ALL2ONE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a3df21ad969f11d269345ce30720692f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_COLL_ALL2ALL</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096abcd942a8acb9457cea8202686acbcbe7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_COLL_OTHER</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096aff99bdef1be11b98d35b5be0c629d4ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_FILE_IO</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a66ff43e36f7f6547bf9f728f3cfc2493</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_POINT2POINT</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a713d812b1122231025f42bcdd6e2d892</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_RMA</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a850b289eb0065597892cf74642491b13</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_DATA_TRANSFER</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a3984b288d2ae016ba6100b39bde17e39</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_REGION_ROLE_ARTIFICIAL</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>ad6aa4cd3679d4ccc0b76e40638ab0096a62d9fa5aa614372aaae31aa4d7eb34f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_SystemTreeDomain_enum</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>afffc4d25bcdf6c27601e547e4372eebb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SYSTEM_TREE_DOMAIN_MACHINE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>afffc4d25bcdf6c27601e547e4372eebbaed1c22ae53e262e6c6b3065a608aae6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SYSTEM_TREE_DOMAIN_SHARED_MEMORY</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>afffc4d25bcdf6c27601e547e4372eebbad653b71ac3c2bed150602d88aeb086d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SYSTEM_TREE_DOMAIN_NUMA</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>afffc4d25bcdf6c27601e547e4372eebba89dea3b1b7e672810b03a171c8846ddb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SYSTEM_TREE_DOMAIN_SOCKET</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>afffc4d25bcdf6c27601e547e4372eebba5cafa11de213951a29863f0bad253ec6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SYSTEM_TREE_DOMAIN_CACHE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>afffc4d25bcdf6c27601e547e4372eebbac4e8d9761408c5d860582f83072ded35</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SYSTEM_TREE_DOMAIN_CORE</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>afffc4d25bcdf6c27601e547e4372eebba58d8ac992436c76e1ccb786c6d063bd8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SYSTEM_TREE_DOMAIN_PU</name>
      <anchorfile>OTF2__Definitions_8h.html</anchorfile>
      <anchor>afffc4d25bcdf6c27601e547e4372eebba9884c4746734d895426348d7d9e17ce5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_DefReader.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__DefReader_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__Definitions_8h" name="OTF2_Definitions.h" local="no" imported="no">otf2/OTF2_Definitions.h</includes>
    <includes id="OTF2__DefReaderCallbacks_8h" name="OTF2_DefReaderCallbacks.h" local="no" imported="no">otf2/OTF2_DefReaderCallbacks.h</includes>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReader_GetLocationID</name>
      <anchorfile>OTF2__DefReader_8h.html</anchorfile>
      <anchor>a505cd30a8294cb5048edea280f3571a3</anchor>
      <arglist>(const OTF2_DefReader *reader, OTF2_LocationRef *location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReader_ReadDefinitions</name>
      <anchorfile>OTF2__DefReader_8h.html</anchorfile>
      <anchor>ab433d36e0a910a56612d86dcbc242cf5</anchor>
      <arglist>(OTF2_DefReader *reader, uint64_t recordsToRead, uint64_t *recordsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReader_SetCallbacks</name>
      <anchorfile>OTF2__DefReader_8h.html</anchorfile>
      <anchor>a975b75cd2dde701adbb9ab125f9c07d7</anchor>
      <arglist>(OTF2_DefReader *reader, const OTF2_DefReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_DefReaderCallbacks.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__DefReaderCallbacks_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__GeneralDefinitions_8h" name="OTF2_GeneralDefinitions.h" local="no" imported="no">otf2/OTF2_GeneralDefinitions.h</includes>
    <includes id="OTF2__Definitions_8h" name="OTF2_Definitions.h" local="no" imported="no">otf2/OTF2_Definitions.h</includes>
    <includes id="OTF2__IdMap_8h" name="OTF2_IdMap.h" local="no" imported="no">otf2/OTF2_IdMap.h</includes>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_Attribute</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a2268e94bdc28b35b4ed05689d6b25846</anchor>
      <arglist>)(void *userData, OTF2_AttributeRef self, OTF2_StringRef name, OTF2_Type type)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_Callpath</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a549dea69bda8d4faee8409298898d479</anchor>
      <arglist>)(void *userData, OTF2_CallpathRef self, OTF2_CallpathRef parent, OTF2_RegionRef region)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_Callsite</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>af565286b9466164f56ce531ca0c9c582</anchor>
      <arglist>)(void *userData, OTF2_CallsiteRef self, OTF2_StringRef sourceFile, uint32_t lineNumber, OTF2_RegionRef enteredRegion, OTF2_RegionRef leftRegion)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_ClockOffset</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>abe347eeed347b380ee88ad98874caefd</anchor>
      <arglist>)(void *userData, OTF2_TimeStamp time, int64_t offset, double standardDeviation)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_Comm</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a40f63b84d0ff21347ca2fc427208b090</anchor>
      <arglist>)(void *userData, OTF2_CommRef self, OTF2_StringRef name, OTF2_GroupRef group, OTF2_CommRef parent)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_Group</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a6fb5df514c190780d75f386fb755045e</anchor>
      <arglist>)(void *userData, OTF2_GroupRef self, OTF2_StringRef name, OTF2_GroupType groupType, OTF2_Paradigm paradigm, OTF2_GroupFlag groupFlags, uint32_t numberOfMembers, const uint64_t *members)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_Location</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a4a63e15f8ad0ad02d068ebd85fbd461a</anchor>
      <arglist>)(void *userData, OTF2_LocationRef self, OTF2_StringRef name, OTF2_LocationType locationType, uint64_t numberOfEvents, OTF2_LocationGroupRef locationGroup)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_LocationGroup</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a4d610e74c3ed811ddaaced9359d101f7</anchor>
      <arglist>)(void *userData, OTF2_LocationGroupRef self, OTF2_StringRef name, OTF2_LocationGroupType locationGroupType, OTF2_SystemTreeNodeRef systemTreeParent)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_MappingTable</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a5e90b8e3c018a0469d1ce318e0bdcf86</anchor>
      <arglist>)(void *userData, OTF2_MappingType mappingType, const OTF2_IdMap *idMap)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_MetricClass</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>ad4bb2f6d05a804edcee0a2b7e9b10749</anchor>
      <arglist>)(void *userData, OTF2_MetricRef self, uint8_t numberOfMetrics, const OTF2_MetricMemberRef *metricMembers, OTF2_MetricOccurrence metricOccurrence, OTF2_RecorderKind recorderKind)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_MetricClassRecorder</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a1cc0a4ae368aaee0a3b6b0a547ec943e</anchor>
      <arglist>)(void *userData, OTF2_MetricRef metricClass, OTF2_LocationRef recorder)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_MetricInstance</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a4ebabd657651d479e02ed53d76210d4c</anchor>
      <arglist>)(void *userData, OTF2_MetricRef self, OTF2_MetricRef metricClass, OTF2_LocationRef recorder, OTF2_MetricScope metricScope, uint64_t scope)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_MetricMember</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a33cc6079d43811cc67564c8748cdf478</anchor>
      <arglist>)(void *userData, OTF2_MetricMemberRef self, OTF2_StringRef name, OTF2_StringRef description, OTF2_MetricType metricType, OTF2_MetricMode metricMode, OTF2_Type valueType, OTF2_MetricBase metricBase, int64_t exponent, OTF2_StringRef unit)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_Parameter</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a573ed0118c3b101619b5be71c49df4ac</anchor>
      <arglist>)(void *userData, OTF2_ParameterRef self, OTF2_StringRef name, OTF2_ParameterType parameterType)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_Region</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a452b995af0a4a227632d56c63b03ee5c</anchor>
      <arglist>)(void *userData, OTF2_RegionRef self, OTF2_StringRef name, OTF2_StringRef canonicalName, OTF2_StringRef description, OTF2_RegionRole regionRole, OTF2_Paradigm paradigm, OTF2_RegionFlag regionFlags, OTF2_StringRef sourceFile, uint32_t beginLineNumber, uint32_t endLineNumber)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_RmaWin</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>aeaf161cfb17b13ca043b76b895162672</anchor>
      <arglist>)(void *userData, OTF2_RmaWinRef self, OTF2_StringRef name, OTF2_CommRef comm)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_String</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a141df67c44bfd3f3fda622bbd1dbe0d7</anchor>
      <arglist>)(void *userData, OTF2_StringRef self, const char *string)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_SystemTreeNode</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a23f03985e69441327e2bfd73115c2d34</anchor>
      <arglist>)(void *userData, OTF2_SystemTreeNodeRef self, OTF2_StringRef name, OTF2_StringRef className, OTF2_SystemTreeNodeRef parent)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_SystemTreeNodeDomain</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>abd179eb25eb1b17fbcf3664b44bb46f5</anchor>
      <arglist>)(void *userData, OTF2_SystemTreeNodeRef systemTreeNode, OTF2_SystemTreeDomain systemTreeDomain)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_SystemTreeNodeProperty</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a9ea5f129cb06d7774656242432658c60</anchor>
      <arglist>)(void *userData, OTF2_SystemTreeNodeRef systemTreeNode, OTF2_StringRef name, OTF2_StringRef value)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_DefReaderCallback_Unknown</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a2c18002575546b198511021300893659</anchor>
      <arglist>)(void *userData)</arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_DefReaderCallbacks_struct</type>
      <name>OTF2_DefReaderCallbacks</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>ac55efed306bd1839fef8d96cf0224d42</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_DefReaderCallbacks_Clear</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a16388777cfb8cf969bf3b8134325c012</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_DefReaderCallbacks_Delete</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a502ce878d0f2993f89ce5ba80a63b0eb</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_DefReaderCallbacks *</type>
      <name>OTF2_DefReaderCallbacks_New</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>aaa2eea63047e7147dc7829cbb53e480d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetAttributeCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a8b747ec60b7b791ebff06ebd913818d4</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_Attribute attributeCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetCallpathCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>abb3e73cb4eb3f68460cc24fb2f64deb4</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_Callpath callpathCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetCallsiteCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a469fd1b2abad68cc905bf67b38143220</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_Callsite callsiteCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetClockOffsetCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a9de9153d2ce1e1a4d335bd1db917590c</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_ClockOffset clockOffsetCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetCommCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a4a3d9841e8fed02714ebf4bec86517c5</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_Comm commCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetGroupCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>ae18a39ad67200ac3e82d0115dd0e3e57</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_Group groupCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetLocationCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a81c6c5cd37cc9e438404f0bb3874dfb8</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_Location locationCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetLocationGroupCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>ad4b36d3d05343c7f9db6ac19bc84de44</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_LocationGroup locationGroupCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetMappingTableCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a856544532a3b245d73147a64d0748265</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_MappingTable mappingTableCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetMetricClassCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a003528065522682ff66c06484dfd9529</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_MetricClass metricClassCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetMetricClassRecorderCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a4027a1d8a459aa5199a9aec3c183d2a9</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_MetricClassRecorder metricClassRecorderCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetMetricInstanceCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a269317ee438ffe626927530686ade100</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_MetricInstance metricInstanceCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetMetricMemberCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a74b7e1e12771a68f238be78e435ac16b</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_MetricMember metricMemberCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetParameterCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a24c4d1b3b8e6009df3a1c319be5d6093</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_Parameter parameterCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetRegionCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>ab685f2f1b071e555d4b5fcd8bb23597b</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_Region regionCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetRmaWinCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a71cdb9dab05d7c52681e47e20148957b</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_RmaWin rmaWinCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetStringCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a44a7ee9ccb42c375e603b3ffb75922e8</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_String stringCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetSystemTreeNodeCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a3dae85bfd698496d581cadf7325b9d04</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_SystemTreeNode systemTreeNodeCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetSystemTreeNodeDomainCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a84b76b72d9b9cefbeeceb42946eddb02</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_SystemTreeNodeDomain systemTreeNodeDomainCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetSystemTreeNodePropertyCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a7463ca2db02bea069752ce8311d161ce</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_SystemTreeNodeProperty systemTreeNodePropertyCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefReaderCallbacks_SetUnknownCallback</name>
      <anchorfile>OTF2__DefReaderCallbacks_8h.html</anchorfile>
      <anchor>a8c966a4a2f9b5e6f58e61c233453d63f</anchor>
      <arglist>(OTF2_DefReaderCallbacks *defReaderCallbacks, OTF2_DefReaderCallback_Unknown unknownCallback)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_DefWriter.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__DefWriter_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__Definitions_8h" name="OTF2_Definitions.h" local="no" imported="no">otf2/OTF2_Definitions.h</includes>
    <includes id="OTF2__IdMap_8h" name="OTF2_IdMap.h" local="no" imported="no">otf2/OTF2_IdMap.h</includes>
    <member kind="typedef">
      <type>struct OTF2_DefWriter_struct</type>
      <name>OTF2_DefWriter</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a8063485b0e755c1cb73f0c28a81bd105</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_GetLocationID</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a07ddd45ffcb5072a323c1e90c812df1f</anchor>
      <arglist>(const OTF2_DefWriter *writer, OTF2_LocationRef *location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteAttribute</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>ab5f443798c8b344ffa96f304cb02760b</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_AttributeRef self, OTF2_StringRef name, OTF2_Type type)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteCallpath</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a798165672799d0035b751f8ccc7be08f</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_CallpathRef self, OTF2_CallpathRef parent, OTF2_RegionRef region)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteCallsite</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>ab918406dfc0740ef3b8ec93db66af859</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_CallsiteRef self, OTF2_StringRef sourceFile, uint32_t lineNumber, OTF2_RegionRef enteredRegion, OTF2_RegionRef leftRegion)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteClockOffset</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a7527d5e2cd76db6c9fe12d69ed479537</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_TimeStamp time, int64_t offset, double standardDeviation)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteComm</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a2ee8936a8244a9e24529f751003c15be</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_CommRef self, OTF2_StringRef name, OTF2_GroupRef group, OTF2_CommRef parent)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteGroup</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a52ffe800c0a07b848605223ba52238a5</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_GroupRef self, OTF2_StringRef name, OTF2_GroupType groupType, OTF2_Paradigm paradigm, OTF2_GroupFlag groupFlags, uint32_t numberOfMembers, const uint64_t *members)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteLocation</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a7ac46983b3f7aa59e3ecc2bb9ac8a9dc</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_LocationRef self, OTF2_StringRef name, OTF2_LocationType locationType, uint64_t numberOfEvents, OTF2_LocationGroupRef locationGroup)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteLocationGroup</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a1353ae8bd4d23fcc974dc2684337255f</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_LocationGroupRef self, OTF2_StringRef name, OTF2_LocationGroupType locationGroupType, OTF2_SystemTreeNodeRef systemTreeParent)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteMappingTable</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a4dc4eaf206d5bd8dccd6c6e37698fa2d</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_MappingType mappingType, const OTF2_IdMap *idMap)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteMetricClass</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a12858b164707fd84f6476231fad5e1ad</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_MetricRef self, uint8_t numberOfMetrics, const OTF2_MetricMemberRef *metricMembers, OTF2_MetricOccurrence metricOccurrence, OTF2_RecorderKind recorderKind)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteMetricClassRecorder</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>adeb038d4fc368c8bc8b3f51d8a8962b1</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_MetricRef metricClass, OTF2_LocationRef recorder)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteMetricInstance</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a3ac81318f0dc4a4c70c535bd56532c14</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_MetricRef self, OTF2_MetricRef metricClass, OTF2_LocationRef recorder, OTF2_MetricScope metricScope, uint64_t scope)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteMetricMember</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>aedfaca99cf7db67ac51b980accde8415</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_MetricMemberRef self, OTF2_StringRef name, OTF2_StringRef description, OTF2_MetricType metricType, OTF2_MetricMode metricMode, OTF2_Type valueType, OTF2_MetricBase metricBase, int64_t exponent, OTF2_StringRef unit)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteParameter</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a2ab1141e35dc7b666a2338d3963668f0</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_ParameterRef self, OTF2_StringRef name, OTF2_ParameterType parameterType)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteRegion</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a55d162f061360a5d2fb33127ee730f3c</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_RegionRef self, OTF2_StringRef name, OTF2_StringRef canonicalName, OTF2_StringRef description, OTF2_RegionRole regionRole, OTF2_Paradigm paradigm, OTF2_RegionFlag regionFlags, OTF2_StringRef sourceFile, uint32_t beginLineNumber, uint32_t endLineNumber)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteRmaWin</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>aa7a5981088ab1adfbe86ec128724be6d</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_RmaWinRef self, OTF2_StringRef name, OTF2_CommRef comm)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteString</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a748d04ffb1ef8be84ef9c3ef720d8073</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_StringRef self, const char *string)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteSystemTreeNode</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a5c8100acb70ad4538c1b965defc55fe5</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_SystemTreeNodeRef self, OTF2_StringRef name, OTF2_StringRef className, OTF2_SystemTreeNodeRef parent)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteSystemTreeNodeDomain</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a6cbea981722b190aea51bbd7156b3818</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_SystemTreeNodeRef systemTreeNode, OTF2_SystemTreeDomain systemTreeDomain)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_DefWriter_WriteSystemTreeNodeProperty</name>
      <anchorfile>OTF2__DefWriter_8h.html</anchorfile>
      <anchor>a9a338bb7693fcd46ac88e1cd5c206015</anchor>
      <arglist>(OTF2_DefWriter *writer, OTF2_SystemTreeNodeRef systemTreeNode, OTF2_StringRef name, OTF2_StringRef value)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_ErrorCodes.h</name>
    <path>/home/roessel/dev/otf2/build/tags/REL-1.2.1/gcc/include/otf2/</path>
    <filename>OTF2__ErrorCodes_8h</filename>
    <member kind="typedef">
      <type>OTF2_ErrorCode(*</type>
      <name>OTF2_ErrorCallback</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a245506e39d12a0b13de31a7edd65bd3c</anchor>
      <arglist>)(void *userData, const char *file, uint64_t line, const char *function, OTF2_ErrorCode errorCode, const char *msgFormatString, va_list va)</arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_ErrorCode</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_DEPRECATED</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a6fe30f750daac6c885c0d969c58aefbc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ABORT</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a6c41fb39d90bf5913234632d55b2c28b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_WARNING</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1acce55b33d04c82cbb5490ccfc963648f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SUCCESS</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a4cbb35ea78add4a8f9e264f7824de5b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_INVALID</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1afba97c80f9800324b4cf5d2d1a69fafc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_E2BIG</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a9407ddff8ecb4c5ab3e308a783039723</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EACCES</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a7dfa07f3b7f2c20d187ff5dd718f56ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EADDRNOTAVAIL</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a4b11eb770e33caaf957ad5aee5d2b8ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EAFNOSUPPORT</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ab413b06dd4aa9148c3e3fe5ad4214126</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EAGAIN</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a5fbd68716d18df1e2a1d5d421683ee97</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EALREADY</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a9dedd31d85ae3aa84a2dc60fff0910e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EBADF</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1aefe63cb754d0466128e148302e65423e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EBADMSG</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a0890be8e0c4c731ba4edaf1ca2069f28</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EBUSY</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a6436e7ed3c0c7a4a8b8937412f79b617</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ECANCELED</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ab6555ae1e5c718889f0b26e9b61674f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ECHILD</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1aeca683d3b0281544dc6178a3ac363a5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ECONNREFUSED</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a6bb9c45268bb529e75b9836e3b91c30d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ECONNRESET</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a9b719fc4d76dce5a65f8b692babdd42a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EDEADLK</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a12739b0e61c56a7fb74e45611063c2a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EDESTADDRREQ</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ad9fc52a3df716713c0c7fb06b19c6e5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EDOM</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a9b45292da297f5b33becc1b6e8d3bdf4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EDQUOT</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a664dae7d8ad06de620b64dd33e0d4f71</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EEXIST</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a8923816d65b3a2ccf918fcda1eb907a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EFAULT</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a409b6948f6bb9706d048e85f0c19c8e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EFBIG</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a54b1f634b2a49cd4652adc7549cf69c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EINPROGRESS</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a18939d87e717f2055cb8a90974120bbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EINTR</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a4673f19d0dd398553e08ba7c3de0105f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EINVAL</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a1daf8ede2fdee01d51f1663069cce216</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EIO</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1adef3749ef8151532049a148a1ceb9597</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EISCONN</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a6185f25d8ed8419d2e77aab98ef404fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EISDIR</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1abb12d2f9b789f5c6c8dad986e8f0612b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ELOOP</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ad8d5de0880f7c3259861ea19df689842</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EMFILE</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1aaabbe839d9d1ca4fa201f79f645c2a4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EMLINK</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a5b9e12f6e620b0095e52004031a33840</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EMSGSIZE</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a0eeb68230f49d1036170428d7fba7cdd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EMULTIHOP</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a95f95eb6fb2c64dbcc49d62e178811d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENAMETOOLONG</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1afaf52e111a0ba3bf4fcf77122e0676c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENETDOWN</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1aba2595dd870df9dc0152f589022109af</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENETRESET</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1aaca7394f302c265ccb2a21c18690bf9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENETUNREACH</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1acc7e0eb386fea96b3f9c92dd84583d45</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENFILE</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a60a42d31c5f1efffcf9328ca536269d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOBUFS</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1aef877387baa50a17363eaa0a46e65ca2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENODATA</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1aeb82568c55fdbd7b35dd54a3f87e83a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENODEV</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a8e7498de61fddca6f3136d5decde4927</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOENT</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a221b36280de3123e63d746df83ba57b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOEXEC</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a49a105ff2a7d6a3a7cbbd0b86be2cc11</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOLCK</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ac31da63146dab30d127a8d3c09e03e6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOLINK</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a9d560a83984487093bb9cd417657178e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOMEM</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a58c2abc5720d14ae2634460f6382cff4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOMSG</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a111ddf18d6a4addceb52b93086bbe021</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOPROTOOPT</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a7f887e4b8b570fcac2991f9a504bcf6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOSPC</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1aba1906f0f181191e2a1b5d2c3c4929ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOSR</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a1e894ea2c01a69b3241f58320c489a97</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOSTR</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a8e6ba1b5b2a4cb638a123601139b3089</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOSYS</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a380c507ddea33d1f0a3106e830a6669b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOTCONN</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1af875635bb163c5d13914b6a0cbc393af</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOTDIR</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a62047d6194a88705ce82a98bdafdf0f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOTEMPTY</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ac485c46d3c9bba34c322ce8d9476a009</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOTSOCK</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ad1d1d544363781c994c049861dfb93ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOTSUP</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1acef3bfeace3b56ddfa89a4670ccd98ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENOTTY</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a65ea0ca90a45948ada48a5159b90e018</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ENXIO</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a1e0aead09f010ac9a7bba0f80240c6f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EOPNOTSUPP</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ab1dab78ceeb3afcc507852578cf1393e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EOVERFLOW</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ad5dc5b0ceb86cce625e89b40f1206934</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EPERM</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1afd1f324b624fa3f73b78d5d245084c19</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EPIPE</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ab6ef195a8289c9ab24b525d6c24466c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EPROTO</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a5fc034668a5e60796e04026241a277ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EPROTONOSUPPORT</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a3facc2154075e0c56b890c2eaf625561</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EPROTOTYPE</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a45b6ffd0dc81f6b7623e1b7019fab720</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ERANGE</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ac52467b5089424c625920b22dfbab383</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EROFS</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a306d5cb17051233c6e33fadbbf5c2c79</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ESPIPE</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1aa4b1a94a1b1bcb166a472b3e8f0d0501</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ESRCH</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ab928d553b26e91f98824c7c4fa46b9af</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ESTALE</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a9cf894c0ed4ed3a8b034460a0f64013a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ETIME</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a3c51527a4fa129227d3814ca5274278c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ETIMEDOUT</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a54dda14d0259cb04d6a8df2f6394538c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_ETXTBSY</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a8a16673351520cd1539e6af6b7ce702c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EWOULDBLOCK</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1aee2a04256824131c5dbcf2102682e5c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_EXDEV</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1af685791d8762123953cdbecb98cae05e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_END_OF_FUNCTION</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a4d5accb1a25492cdde7262ff1ffa3cc8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_INVALID_CALL</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ad0fbc882b53fd0ed1419cda11b744397</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_INVALID_ARGUMENT</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a1b3f79076a5e18f81bf7b38925a41317</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_INVALID_RECORD</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a5f44bdb0693df600098bb14d6224644a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_INVALID_DATA</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a80205c8dbbb5a6a77607edf1f7694aa0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_INVALID_SIZE_GIVEN</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a8463b5f4d5c95b824826eb2a57315840</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_UNKNOWN_TYPE</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a337c2d9b123aea39b458cf6f5b1db856</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_INTEGRITY_FAULT</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ae075318c7c9cc763e693b4b572e96717</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_MEM_FAULT</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a20be495da2892d74e33a18eb951bb97d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_MEM_ALLOC_FAILED</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1abcf888ffd250f6169df91669a6f89d27</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_PROCESSED_WITH_FAULTS</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a2b113f841229c9968719eef31a49eb3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_INDEX_OUT_OF_BOUNDS</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a5fa81ce9341f394cf445d5f432f6701c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_INVALID_LINENO</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a6476b5cbeb61dc76b3778f2a6b1a2d15</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_END_OF_BUFFER</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a7463e7f3432e0796390174ee110abe41</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_FILE_INTERACTION</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a46732c9f3e1ff36f69780c1bebc263e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_FILE_CAN_NOT_OPEN</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a14c8f8099206d5356ef0f2c6799bbd2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_INTERRUPTED_BY_CALLBACK</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a11414806c8e433197e43982324243acf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_PROPERTY_NAME_INVALID</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1aced94e8d5949bc7f5262cea2b9406e13</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_PROPERTY_EXISTS</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a5bd9fc095e5a549a0d787eff7b8044c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_PROPERTY_NOT_FOUND</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1aa0acdf6a2d9b1846d419228c19f6b304</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_PROPERTY_VALUE_INVALID</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a074351d02c8a26599899c56645d343b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_FILE_COMPRESSION_NOT_SUPPORTED</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a0ef8992163d30ffa90f79d5d18dfa89e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_DUPLICATE_MAPPING_TABLE</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1ad345d1999a91bea9746088cb0871dabd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ERROR_INVALID_FILE_MODE_TRANSITION</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a88338b3aa85934ec8dbc972f109f49f1a0da2df5d40606da9e6e0580dab12aae5</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>OTF2_Error_GetDescription</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>aa86366a6dcf0e3c3279f475678cbf3a7</anchor>
      <arglist>(OTF2_ErrorCode errorCode)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>OTF2_Error_GetName</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a075b64e013c915f5da60b2301060b08c</anchor>
      <arglist>(OTF2_ErrorCode errorCode)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCallback</type>
      <name>OTF2_Error_RegisterCallback</name>
      <anchorfile>OTF2__ErrorCodes_8h.html</anchorfile>
      <anchor>a2a8434969f424aee6edac196e91d9d1a</anchor>
      <arglist>(OTF2_ErrorCallback errorCallbackIn, void *userData)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_Events.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__Events_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__GeneralDefinitions_8h" name="OTF2_GeneralDefinitions.h" local="no" imported="no">otf2/OTF2_GeneralDefinitions.h</includes>
    <class kind="union">OTF2_MetricValue_union</class>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_CollectiveOp</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a331898a7616469ab68fcc31cbe402f4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_LockType</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a5188205207a93107d92000742ff0292e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_MeasurementMode</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a07523806c1165c90230b2e5827f94243</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_RmaAtomicType</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a53549cd80099e4cce5b3d0cb1967442a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_RmaSyncLevel</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>aaa7e8bfced5c5a53c98f9e34f9334f36</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_RmaSyncType</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a2e08d41d5987cf40fb03d477efd0c517</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_CollectiveOp_enum</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a50569e1867cb385000b5be50ace6e2d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_COLLECTIVE_OP_CREATE_HANDLE</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a50569e1867cb385000b5be50ace6e2d3a9805dbd1ac7645d125da41bfb10f1c57</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_COLLECTIVE_OP_DESTROY_HANDLE</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a50569e1867cb385000b5be50ace6e2d3afd18ad2256077695bc64ba2e7f90686a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_COLLECTIVE_OP_ALLOCATE</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a50569e1867cb385000b5be50ace6e2d3abd148283cf10f80116611e1dff1b52ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_COLLECTIVE_OP_DEALLOCATE</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a50569e1867cb385000b5be50ace6e2d3adbc93c35702cc4413e01684b22b917ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_COLLECTIVE_OP_CREATE_HANDLE_AND_ALLOCATE</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a50569e1867cb385000b5be50ace6e2d3a03369208c279a80171d12651b2fdb216</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_COLLECTIVE_OP_DESTROY_HANDLE_AND_DEALLOCATE</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a50569e1867cb385000b5be50ace6e2d3a060eb72d2eeb363256d17d83fa6d14d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_LockType_enum</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a07113e62c2a4b44bca5d4f5dbc80eec5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_LOCK_EXCLUSIVE</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a07113e62c2a4b44bca5d4f5dbc80eec5aa4573e0e194da3fd50e38c5a933467d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_LOCK_SHARED</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a07113e62c2a4b44bca5d4f5dbc80eec5a276deafe759f0b4c62dd17d52c792a80</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_MeasurementMode_enum</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>af48f4fb7ad22b952d288c689e01a687b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MEASUREMENT_ON</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>af48f4fb7ad22b952d288c689e01a687ba4045b185c4e46fc25566628c2ff12253</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MEASUREMENT_OFF</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>af48f4fb7ad22b952d288c689e01a687ba56dc99430b010838d593a81fadbe4969</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_RmaAtomicType_enum</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a4cce49b11e7495a5709691cc8a08422f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_RmaSyncLevel_enum</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a01504aaa23c74659f74cdd4b4a64bef3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_RMA_SYNC_LEVEL_NONE</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a01504aaa23c74659f74cdd4b4a64bef3ad3c171ba5ef48f5184ece205abec9de0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_RMA_SYNC_LEVEL_PROCESS</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a01504aaa23c74659f74cdd4b4a64bef3ac7b0c18fa3b38adab9571761bc423c6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_RMA_SYNC_LEVEL_MEMORY</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>a01504aaa23c74659f74cdd4b4a64bef3ac2b21361ab6cf137f7fab09544025db2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_RmaSyncType_enum</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>ac7af493bf1a05beb3e3b3ed9320bd9ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_RMA_SYNC_TYPE_MEMORY</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>ac7af493bf1a05beb3e3b3ed9320bd9adae20ae401f71c6e1f7a418846cf3fdbde</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_RMA_SYNC_TYPE_NOTIFY_IN</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>ac7af493bf1a05beb3e3b3ed9320bd9ada0b33a24261e8b61cd617a4b811f1eb1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_RMA_SYNC_TYPE_NOTIFY_OUT</name>
      <anchorfile>OTF2__Events_8h.html</anchorfile>
      <anchor>ac7af493bf1a05beb3e3b3ed9320bd9ada3f8c851bd8fba7639e966f6a7c314673</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_EvtReader.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__EvtReader_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__Events_8h" name="OTF2_Events.h" local="no" imported="no">otf2/OTF2_Events.h</includes>
    <includes id="OTF2__Definitions_8h" name="OTF2_Definitions.h" local="no" imported="no">otf2/OTF2_Definitions.h</includes>
    <includes id="OTF2__AttributeList_8h" name="OTF2_AttributeList.h" local="no" imported="no">otf2/OTF2_AttributeList.h</includes>
    <includes id="OTF2__EvtReaderCallbacks_8h" name="OTF2_EvtReaderCallbacks.h" local="no" imported="no">otf2/OTF2_EvtReaderCallbacks.h</includes>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReader_ApplyClockOffsets</name>
      <anchorfile>OTF2__EvtReader_8h.html</anchorfile>
      <anchor>abbe8f0da1c25ce5e54d46453cb3ab9f4</anchor>
      <arglist>(OTF2_EvtReader *reader, bool action)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReader_ApplyMappingTables</name>
      <anchorfile>OTF2__EvtReader_8h.html</anchorfile>
      <anchor>ab1692cdf4a36e3051d18d53840850ff6</anchor>
      <arglist>(OTF2_EvtReader *reader, bool action)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReader_GetLocationID</name>
      <anchorfile>OTF2__EvtReader_8h.html</anchorfile>
      <anchor>a2bb1f755b19a4c776ece02a381252a0d</anchor>
      <arglist>(const OTF2_EvtReader *reader, OTF2_LocationRef *location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReader_GetPos</name>
      <anchorfile>OTF2__EvtReader_8h.html</anchorfile>
      <anchor>a598fdd5137bbb24207ba271a34a170a2</anchor>
      <arglist>(OTF2_EvtReader *reader, uint64_t *position)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReader_ReadEvents</name>
      <anchorfile>OTF2__EvtReader_8h.html</anchorfile>
      <anchor>a0c74490bfdf9463def60e2491390d76b</anchor>
      <arglist>(OTF2_EvtReader *reader, uint64_t recordsToRead, uint64_t *recordsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReader_ReadEventsBackward</name>
      <anchorfile>OTF2__EvtReader_8h.html</anchorfile>
      <anchor>a9e4ef3e1bc34db4da7153ec7064f3dd6</anchor>
      <arglist>(OTF2_EvtReader *reader, uint64_t recordsToRead, uint64_t *recordsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReader_Seek</name>
      <anchorfile>OTF2__EvtReader_8h.html</anchorfile>
      <anchor>ae7d738bfad5c50443d2c288036de2c96</anchor>
      <arglist>(OTF2_EvtReader *reader, uint64_t position)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReader_SetCallbacks</name>
      <anchorfile>OTF2__EvtReader_8h.html</anchorfile>
      <anchor>a53fd76628a8cfee88bcd533bf4b4e480</anchor>
      <arglist>(OTF2_EvtReader *reader, const OTF2_EvtReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReader_TimeStampRewrite</name>
      <anchorfile>OTF2__EvtReader_8h.html</anchorfile>
      <anchor>a4670b9465d4687b8d5de1ce2699e859a</anchor>
      <arglist>(OTF2_EvtReader *reader, OTF2_TimeStamp time)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_EvtReaderCallbacks.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__EvtReaderCallbacks_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__GeneralDefinitions_8h" name="OTF2_GeneralDefinitions.h" local="no" imported="no">otf2/OTF2_GeneralDefinitions.h</includes>
    <includes id="OTF2__AttributeList_8h" name="OTF2_AttributeList.h" local="no" imported="no">otf2/OTF2_AttributeList.h</includes>
    <includes id="OTF2__Events_8h" name="OTF2_Events.h" local="no" imported="no">otf2/OTF2_Events.h</includes>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_BufferFlush</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a02f4700f7f93c4a0388a3489a4078ec5</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp stopTime)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_Enter</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ad40626501522b2557abfc1fbdd25bc02</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RegionRef region)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_Leave</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a95c5d9a4d97d68989ba0937692ac9aae</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RegionRef region)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_MeasurementOnOff</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a596c4314f9dc5fd4d5bc813d7dd7848d</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_MeasurementMode measurementMode)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_Metric</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a6f0eca11f990ba841896b0bb1b4ee56d</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_MetricRef metric, uint8_t numberOfMetrics, const OTF2_Type *typeIDs, const OTF2_MetricValue *metricValues)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_MpiCollectiveBegin</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a617d5cc6b2f0ed4a021fa3e28c2b8e25</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_MpiCollectiveEnd</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a55c36fc1351e136f250766c6be793333</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_CollectiveOp collectiveOp, OTF2_CommRef communicator, uint32_t root, uint64_t sizeSent, uint64_t sizeReceived)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_MpiIrecv</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a1144c7865ef90b08483a6a54a39ee521</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint32_t sender, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_MpiIrecvRequest</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a32d2923f6ffc2b5083c0b51835a42720</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_MpiIsend</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a0974dde372f2d8fe61c90dc794dfbf62</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint32_t receiver, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_MpiIsendComplete</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ac096ebedf121a725dfa671200af5179d</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_MpiRecv</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a7767c474170bf789bb0523a1ef4d3b2b</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint32_t sender, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_MpiRequestCancelled</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a2016c6febbaf66c292a3ba3e14ba2a69</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_MpiRequestTest</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a04cfd29b98dd68f8077796f5f2d420c0</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_MpiSend</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>af2c8ddc6a06bf86030d8e4dfd93faae4</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint32_t receiver, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_OmpAcquireLock</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a5fb959c96c11a9c7bfd8c52251e1c75b</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_OmpFork</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>af620b21e6a997f06dba77d3549f3065f</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint32_t numberOfRequestedThreads)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_OmpJoin</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a8b42b500fa395213900643f93b94dfc8</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_OmpReleaseLock</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ae341fb77fc9e4f13a8ce6607666510d6</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_OmpTaskComplete</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ae4f5cacc4b773c1cff4ebb1a39669f5a</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint64_t taskID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_OmpTaskCreate</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a6adf7c5cc9041d3cac350d6a0e2e1be5</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint64_t taskID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_OmpTaskSwitch</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ab9eff0bb2054666be866a56103440f4d</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, uint64_t taskID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_ParameterInt</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ab075dc2b9e92500633a9ff34dc29f3c8</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_ParameterRef parameter, int64_t value)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_ParameterString</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a54afa857b1bb469da7ade018f1514c7a</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_ParameterRef parameter, OTF2_StringRef string)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_ParameterUnsignedInt</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ab995eabc13b091a67dec3c39e84c09a4</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_ParameterRef parameter, uint64_t value)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaAcquireLock</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a91c7a76ffc5e06c57827dd8788ece800</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, uint64_t lockId, OTF2_LockType lockType)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaAtomic</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a7641f9873c94b73ad412fe828ecf688a</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, OTF2_RmaAtomicType type, uint64_t bytesSent, uint64_t bytesReceived, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaCollectiveBegin</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a11e0c4e688edc9f4f7339640013f21d2</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaCollectiveEnd</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a51cf9dd130e43cef3b81ef5ed5c710c8</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_CollectiveOp collectiveOp, OTF2_RmaSyncLevel syncLevel, OTF2_RmaWinRef win, uint32_t root, uint64_t bytesSent, uint64_t bytesReceived)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaGet</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aca75df2ca80dec2609e0d61e1a0b36eb</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, uint64_t bytes, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaGroupSync</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ab33e56d5788b7e53c88a4624d74ca1da</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaSyncLevel syncLevel, OTF2_RmaWinRef win, OTF2_GroupRef group)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaOpCompleteBlocking</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ae68d38ab80e8a93274470e8fffaf1500</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaOpCompleteNonBlocking</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ac7ec2049b2aad7138825c9a76eb7f9ca</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaOpCompleteRemote</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a3489b74aa75c9902af14081480677e03</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaOpTest</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a31433bd7e1026d78f1f2eff2df38e6d5</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaPut</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a31bd12d9f80f269803a8eae2228ba956</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, uint64_t bytes, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaReleaseLock</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>abfecb774ece7dbd18f4e10386d9f86e7</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, uint64_t lockId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaRequestLock</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ac9529296511ddd04c303b09b1e754e8d</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, uint64_t lockId, OTF2_LockType lockType)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaSync</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a2ff8899fd8150cfde174d1771d807ebb</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, OTF2_RmaSyncType syncType)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaTryLock</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a7f955016fc9b942cbae93acfae68dee5</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, uint64_t lockId, OTF2_LockType lockType)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaWaitChange</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ab57e1a1bd43d8ef62d88937c326d90ec</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaWinCreate</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a703539b79e69d85fec3dfd8deca312b5</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_RmaWinDestroy</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a415faef1532bbf6881d0e39bea7254a7</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_ThreadAcquireLock</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a4e76879835aef2726244fdb18fd622a3</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_Paradigm model, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_ThreadFork</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ac1e182a3ad7781272b0436fb9fa8d3a4</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_Paradigm model, uint32_t numberOfRequestedThreads)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_ThreadJoin</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a11df08c2d7c32535e0b99a7acba85936</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_Paradigm model)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_ThreadReleaseLock</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a045f801f5a940e0965ed83f2690501ae</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_Paradigm model, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_ThreadTaskComplete</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a034cdf6488e64d6f7501f6d0e6348169</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_CommRef threadTeam, uint32_t creatingThread, uint32_t generationNumber)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_ThreadTaskCreate</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a35ed0b56a6d5d01b7e17807d8b8c3e55</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_CommRef threadTeam, uint32_t creatingThread, uint32_t generationNumber)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_ThreadTaskSwitch</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a118383d80d0287866b734fa632219f5b</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_CommRef threadTeam, uint32_t creatingThread, uint32_t generationNumber)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_ThreadTeamBegin</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a4bba9a2b4f409d703df37bdc8a898664</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_CommRef threadTeam)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_ThreadTeamEnd</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aab4ff10245bf5499080ba5d0f2cb4faa</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList, OTF2_CommRef threadTeam)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_EvtReaderCallback_Unknown</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aba660d815910c0c1eacf717cdce307d2</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp time, uint64_t eventPosition, void *userData, OTF2_AttributeList *attributeList)</arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_EvtReaderCallbacks_struct</type>
      <name>OTF2_EvtReaderCallbacks</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a68b24b6b4a17aac8974790222ade4a98</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_EvtReaderCallbacks_Clear</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a7085f5e61b0798caa7f1b6b8301915ac</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_EvtReaderCallbacks_Delete</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ad1267422460d762b990f698dd44a6fe1</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_EvtReaderCallbacks *</type>
      <name>OTF2_EvtReaderCallbacks_New</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a27a0573f7f99b4ce8510a0000e18d891</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetBufferFlushCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ae32a4ec9f2548e777f2018e16d8947c9</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_BufferFlush bufferFlushCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetEnterCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aecb1bed6b5b668bba613562e08d3b325</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_Enter enterCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetLeaveCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aa01dce796fb6bda81a6a3046ee57b5ae</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_Leave leaveCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetMeasurementOnOffCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aa27d075a7f496bddc47cf34bcc26793d</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_MeasurementOnOff measurementOnOffCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetMetricCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a1ba9fc8e44b52ea576bc84037f790100</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_Metric metricCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetMpiCollectiveBeginCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ac42e41a2f6d22e51ddd254bfb26de72f</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_MpiCollectiveBegin mpiCollectiveBeginCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetMpiCollectiveEndCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a44a4b7b9dd526afefc263408f31fc3a7</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_MpiCollectiveEnd mpiCollectiveEndCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetMpiIrecvCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>afc05eebf69b3f1d56fb7659ecb7616dd</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_MpiIrecv mpiIrecvCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetMpiIrecvRequestCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aa54fb58b81b875ea769580a4c54ca6c4</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_MpiIrecvRequest mpiIrecvRequestCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetMpiIsendCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a060612e10f43edfe55f55bd06446f526</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_MpiIsend mpiIsendCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetMpiIsendCompleteCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ab430a3822072ccb21d150dc9509e3452</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_MpiIsendComplete mpiIsendCompleteCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetMpiRecvCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>abe91e83fc7e74f07dd781e3a86471dba</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_MpiRecv mpiRecvCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetMpiRequestCancelledCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a31e7696d346974966db6981a555b8a30</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_MpiRequestCancelled mpiRequestCancelledCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetMpiRequestTestCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a7c19a050e92cc63f407349b4dde16bb2</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_MpiRequestTest mpiRequestTestCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetMpiSendCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aadd68d314fe8c78b3fd7ba06479986d7</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_MpiSend mpiSendCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetOmpAcquireLockCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aa0448edf15d8fc550a8514bbb6f0cdcd</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_OmpAcquireLock ompAcquireLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetOmpForkCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aeedea7e39b86b18a8b143d5d0e1eb820</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_OmpFork ompForkCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetOmpJoinCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a05f5c97b80b95a0f361066ca0ec34c7c</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_OmpJoin ompJoinCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetOmpReleaseLockCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a3e9d36d62821f3bf95325fb7e5e63ffa</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_OmpReleaseLock ompReleaseLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetOmpTaskCompleteCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aef420925b8a76660360604b8108edcc0</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_OmpTaskComplete ompTaskCompleteCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetOmpTaskCreateCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>af2fdfab310e8a911cf5cb5c52b93f9e5</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_OmpTaskCreate ompTaskCreateCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetOmpTaskSwitchCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a572282e9ad84eb633f30e11a59ceebaa</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_OmpTaskSwitch ompTaskSwitchCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetParameterIntCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a8e7afa0bccf7e02c8a7229096440678b</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_ParameterInt parameterIntCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetParameterStringCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a7a117335084f4f77c06ca90f512097e7</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_ParameterString parameterStringCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetParameterUnsignedIntCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aee586a3f758d52cecd9c310ea4d3940a</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_ParameterUnsignedInt parameterUnsignedIntCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaAcquireLockCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a83eb0e732c34c360805c999c435d48a0</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaAcquireLock rmaAcquireLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaAtomicCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a5cb035faf9c02012f6d2e3ca854a3bd9</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaAtomic rmaAtomicCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaCollectiveBeginCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a3e0864a87260a601f8fe36cea6fa518f</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaCollectiveBegin rmaCollectiveBeginCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaCollectiveEndCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a3f79822fd432c2109d1ae933b37f40e9</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaCollectiveEnd rmaCollectiveEndCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaGetCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a9b2a77d5bf5675339d6ecc61a81b8f98</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaGet rmaGetCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaGroupSyncCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a9ca1c0904b04d84d3c9b6d0e508685e0</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaGroupSync rmaGroupSyncCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaOpCompleteBlockingCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a84f9bba7630669196ba9f69caf2cb4af</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaOpCompleteBlocking rmaOpCompleteBlockingCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaOpCompleteNonBlockingCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a046fce8538f3e482f1c39e758ba42eaf</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaOpCompleteNonBlocking rmaOpCompleteNonBlockingCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaOpCompleteRemoteCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a19b1c05d3d6c5cd7c115d2a2e06333fc</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaOpCompleteRemote rmaOpCompleteRemoteCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaOpTestCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ad589a3c908aec83489ac7348f32a176f</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaOpTest rmaOpTestCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaPutCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a1a02b2dd12cc46bf0c5b72c509a5ca69</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaPut rmaPutCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaReleaseLockCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a1082a3e9978c54dc9d7d4dcbcb38d469</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaReleaseLock rmaReleaseLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaRequestLockCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>afc4a5a67bb034f92f67ad9fb8972f111</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaRequestLock rmaRequestLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaSyncCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a8370d8514e7aa6755bdea52d5f17757c</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaSync rmaSyncCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaTryLockCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a47fa366937bf6a0d7a54c76f5b015d60</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaTryLock rmaTryLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaWaitChangeCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a6ea9b2b159cad43c588182fe8f832289</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaWaitChange rmaWaitChangeCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaWinCreateCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a6d275947d75ca6a7a65cf3171d713890</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaWinCreate rmaWinCreateCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetRmaWinDestroyCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a088a149f4f17d06ab921734c0c0b4ad3</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_RmaWinDestroy rmaWinDestroyCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetThreadAcquireLockCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a39a3dba96fd73d84d0d6c914fd934435</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_ThreadAcquireLock threadAcquireLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetThreadForkCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ad78c84c942c0ad43fe4c27cff164d49d</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_ThreadFork threadForkCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetThreadJoinCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a7fa4c3353c858e331456f6031681a781</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_ThreadJoin threadJoinCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetThreadReleaseLockCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aa97ee857d35ed4c04c8b82dededf44bb</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_ThreadReleaseLock threadReleaseLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetThreadTaskCompleteCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a64fc6479672021f416a06fa8f13614b4</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_ThreadTaskComplete threadTaskCompleteCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetThreadTaskCreateCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ac5ff9785e0d2d1953029e6de2abd5da5</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_ThreadTaskCreate threadTaskCreateCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetThreadTaskSwitchCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aa9cf8cac153bffb6a4bf4900f550d88d</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_ThreadTaskSwitch threadTaskSwitchCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetThreadTeamBeginCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a38463887d155ee525b0a8ed55a7f8dac</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_ThreadTeamBegin threadTeamBeginCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetThreadTeamEndCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a6cc46818bc796b427d140b269837c55e</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_ThreadTeamEnd threadTeamEndCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtReaderCallbacks_SetUnknownCallback</name>
      <anchorfile>OTF2__EvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a7130e09d6bee3b736e1f372eb7ee199a</anchor>
      <arglist>(OTF2_EvtReaderCallbacks *evtReaderCallbacks, OTF2_EvtReaderCallback_Unknown unknownCallback)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_EvtWriter.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__EvtWriter_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__Events_8h" name="OTF2_Events.h" local="no" imported="no">otf2/OTF2_Events.h</includes>
    <includes id="OTF2__AttributeList_8h" name="OTF2_AttributeList.h" local="no" imported="no">otf2/OTF2_AttributeList.h</includes>
    <member kind="typedef">
      <type>struct OTF2_EvtWriter_struct</type>
      <name>OTF2_EvtWriter</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>ad737910705153d28865e81d86a2aea0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_BufferFlush</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>ac02f43c41abedfd5b859c8ea090bfb82</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_TimeStamp stopTime)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_ClearRewindPoint</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>ad24209132d4219878ab2fc33b4c29dc6</anchor>
      <arglist>(OTF2_EvtWriter *writer, uint32_t rewindId)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_Enter</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>aa9a392914cc026f06b8fdc3e909c32ef</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RegionRef region)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_GetLocationID</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>ace93c526385519e550e01bf136d2e623</anchor>
      <arglist>(const OTF2_EvtWriter *writer, OTF2_LocationRef *locationID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_GetNumberOfEvents</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>aae58b3b5e941dd0d47a677d04022612d</anchor>
      <arglist>(OTF2_EvtWriter *writer, uint64_t *numberOfEvents)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_GetUserData</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>ab58fcc1144568c555d08f94a678259f7</anchor>
      <arglist>(const OTF2_EvtWriter *writer, void **userData)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_Leave</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a905aa449b645e3100f249af5f91e5279</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RegionRef region)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_MeasurementOnOff</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a1e3944f2744028677a556e0bd6ec10a6</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_MeasurementMode measurementMode)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_Metric</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a41e4c713df9f42bfc0277ccbf1441a0e</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_MetricRef metric, uint8_t numberOfMetrics, const OTF2_Type *typeIDs, const OTF2_MetricValue *metricValues)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_MpiCollectiveBegin</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>ad780931624547248b522af737f50e1d8</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_MpiCollectiveEnd</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a64bb71394d092f079e916e40fb2bf807</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_CollectiveOp collectiveOp, OTF2_CommRef communicator, uint32_t root, uint64_t sizeSent, uint64_t sizeReceived)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_MpiIrecv</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>aba9b65b814e3243e574743407ae22b41</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint32_t sender, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength, uint64_t requestID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_MpiIrecvRequest</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>ab22e9fbd87d79c30074a1f734360be69</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint64_t requestID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_MpiIsend</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a2b9bead6eaca86844977f73efa287b67</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint32_t receiver, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength, uint64_t requestID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_MpiIsendComplete</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>af91755903763fdd2fd72725156565555</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint64_t requestID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_MpiRecv</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a1eb0027b354ee2ab787881e1cbcd8a18</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint32_t sender, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_MpiRequestCancelled</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a7df5e78bc40771f5ce770035672e345e</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint64_t requestID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_MpiRequestTest</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>af97c1af69cd10dc20295a0da8e09ff66</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint64_t requestID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_MpiSend</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a893d33b9fbbb1cb206ad649fd90a6517</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint32_t receiver, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_OmpAcquireLock</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a4ee9074e39d416523be843a470176503</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_OmpFork</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a6c2825f1e577a6350ed594387369b3b4</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint32_t numberOfRequestedThreads)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_OmpJoin</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>ab07bde762daf2fa0c29c2aef1a3063a9</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_OmpReleaseLock</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a8e0cd1ec2fae9d6d447d5799c2376643</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_OmpTaskComplete</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>adc3e7116eafe084b93f5c27606541cc9</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint64_t taskID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_OmpTaskCreate</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a824aea15222405f6d2141ea7905b6904</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint64_t taskID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_OmpTaskSwitch</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>afd5b801dd76a152e0cf1e64a314a523b</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, uint64_t taskID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_ParameterInt</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a125003e2e68dc7a5f0e6ea835b476f79</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_ParameterRef parameter, int64_t value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_ParameterString</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a30a8b3e33c65a6353c39db8886607854</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_ParameterRef parameter, OTF2_StringRef string)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_ParameterUnsignedInt</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>afb8182a83b9f5a43def7cb03cfb7684e</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_ParameterRef parameter, uint64_t value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_Rewind</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a75ee61abcaa991ccd9c2bfba9817d75d</anchor>
      <arglist>(OTF2_EvtWriter *writer, uint32_t rewindId)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaAcquireLock</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>afc687aa303365c18c2e6233d32200af5</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win, uint32_t remote, uint64_t lockId, OTF2_LockType lockType)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaAtomic</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>ae13ce102956228757fccd5b04d1aabe0</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win, uint32_t remote, OTF2_RmaAtomicType type, uint64_t bytesSent, uint64_t bytesReceived, uint64_t matchingId)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaCollectiveBegin</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a02c033e81f5ff43aa41c7c74156a2a71</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaCollectiveEnd</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a4e1d7b0fd7d948ca25adb073dc6cd331</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_CollectiveOp collectiveOp, OTF2_RmaSyncLevel syncLevel, OTF2_RmaWinRef win, uint32_t root, uint64_t bytesSent, uint64_t bytesReceived)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaGet</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a94950c932c4b891d92db64b8c56d8bee</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win, uint32_t remote, uint64_t bytes, uint64_t matchingId)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaGroupSync</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a90a4c703fcd731038457ad3aff8a93c0</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaSyncLevel syncLevel, OTF2_RmaWinRef win, OTF2_GroupRef group)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaOpCompleteBlocking</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a0745cc23275da63c257cf2e6f69b3430</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win, uint64_t matchingId)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaOpCompleteNonBlocking</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a34747c15337aa11911d91d633d6fba4b</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win, uint64_t matchingId)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaOpCompleteRemote</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a4b120d486369f63f5ba1a1ae46c26089</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win, uint64_t matchingId)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaOpTest</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>af6170917126e7e349aef2de982fc2040</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win, uint64_t matchingId)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaPut</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a10efad8d2a9195ebe6a5c404ad197603</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win, uint32_t remote, uint64_t bytes, uint64_t matchingId)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaReleaseLock</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a3697cfbfac15c9905f6023990e5fcb7c</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win, uint32_t remote, uint64_t lockId)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaRequestLock</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>aec01cb5cb96d8b0f10f53f251671483b</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win, uint32_t remote, uint64_t lockId, OTF2_LockType lockType)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaSync</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a7f92958fb40e7f064f823c6b6f3f0f0f</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win, uint32_t remote, OTF2_RmaSyncType syncType)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaTryLock</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a9cf53f11671fd5b5ba3936a7a3849906</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win, uint32_t remote, uint64_t lockId, OTF2_LockType lockType)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaWaitChange</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>ac8367696bbba6e280435fdd65d3fc7c4</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaWinCreate</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a6ee94c2ac88e34f4622a1625d7e6d6a0</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_RmaWinDestroy</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a4b9ae7afe401cf19cc1d7fc58f326d1d</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_RmaWinRef win)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_SetLocationID</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>ae958f69513716fa5a28b52affbb59280</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_LocationRef location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_SetUserData</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a5866660bb4859c2b202a1c11b385e819</anchor>
      <arglist>(OTF2_EvtWriter *writer, void *userData)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_StoreRewindPoint</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a21aedf3f1cf1ceead7185539fcf356fd</anchor>
      <arglist>(OTF2_EvtWriter *writer, uint32_t rewindId)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_ThreadAcquireLock</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a8b2a265ba19732a79743f64c53d469dd</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_Paradigm model, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_ThreadFork</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a63ac265c2e6a37a7d4baeeedfe8b3816</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_Paradigm model, uint32_t numberOfRequestedThreads)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_ThreadJoin</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>af585af8988353afb700b7acd138dec08</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_Paradigm model)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_ThreadReleaseLock</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a344441b082a099fa63d4d8f6242bb9ff</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_Paradigm model, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_ThreadTaskComplete</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a72865c827f481f95fad3e3ecc729e6a3</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_CommRef threadTeam, uint32_t creatingThread, uint32_t generationNumber)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_ThreadTaskCreate</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a980cc15bfd70ca77dcdf7daaa7fca04a</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_CommRef threadTeam, uint32_t creatingThread, uint32_t generationNumber)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_ThreadTaskSwitch</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a538fb88d9ffb6a193b23a11e23b28d2b</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_CommRef threadTeam, uint32_t creatingThread, uint32_t generationNumber)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_ThreadTeamBegin</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a60669c3665abf12ced07e77f891f9bcd</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_CommRef threadTeam)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_EvtWriter_ThreadTeamEnd</name>
      <anchorfile>OTF2__EvtWriter_8h.html</anchorfile>
      <anchor>a9c0896c098454aff81e4f97e66b03f6d</anchor>
      <arglist>(OTF2_EvtWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp time, OTF2_CommRef threadTeam)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_GeneralDefinitions.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__GeneralDefinitions_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_CHUNK_SIZE_MAX</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a541c942e88f8e6c7f685cf5e4b58f01e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_CHUNK_SIZE_MIN</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>ab22a95fcb09e2e1fa8f59118efced7df</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_ATTRIBUTE</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a44a2d43ab15381d77e335e54bead1547</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_CALLPATH</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a65f4e317a856d280a1836b1f56f17e75</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_CALLSITE</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a83a177ba17b6aa79bae54c1a35e2187d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_COMM</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa58c7b4608a20a80781457356958790e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_GROUP</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a4e38db4910ab746d498f8288651845de</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_LOCATION</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a6c5715fcf90c16378ad396dbf2b035a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_LOCATION_GROUP</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a8ffd389af3aecb02cea34c91cf6d4376</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_METRIC</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a4dcc0adc08b525cc547ae570b1440252</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_METRIC_MEMBER</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a5ba235a681074476e64ae54aae74b5d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_PARAMETER</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a8d157e2cc4e500392f8b348557aeb91f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_REGION</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa3372e9da8924155e4c119e16b919da2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_RMA_WIN</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>ada23c4208f2be5c261f7a6bacaa379db</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_STRING</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>afdb5990ddc81a39d2948d08d40bc9091</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_SYSTEM_TREE_NODE</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a78180d2d4378c514f94df5c9093a032e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_TYPE</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>acfed38aa2a2d7c1a9c3dfd955e94108d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_VERSION_MAJOR</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>ac6aaf9a56e9813c52e1f17b70a3b283b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_VERSION_MINOR</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a0956168f60585c3593f0f010521c1a1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_VERSION_BUGFIX</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a853085ca36252b961ab6a175a3943c4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_VERSION_SUFFIX</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a55f856b24fadb2bb491ea0406cc87d63</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_VERSION</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>ad394da7eaac0728e513f7ea8fece6ba6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_UINT8</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>ab3dc8efab41a9d795fc45ee1bd41dd84</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_UINT16</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3327a0253ae1de14266e5437551742b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_UINT32</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a0c3c084a8316482f949edb3d9fd745d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_UINT64</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa2f692a93c3de1d5b478e027d3b1fbcd</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_AttributeRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a9011448ad1eb124141c427c09b168ff5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_CallpathRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>ab54fb99f59cc389b309299478c1c79cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_CallsiteRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>abf2b09ec22f95a74b95319adfb8de8e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_CommRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a4234c205e62715d18fd74d110ddbbbfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_Compression</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3a2f46538162239739a711975c39d8e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_DefReader_struct</type>
      <name>OTF2_DefReader</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>acc06b834be6f70f9ac3691fdb9d20fd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_EvtReader_struct</type>
      <name>OTF2_EvtReader</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a08cb0591a08e2743f971a7a120c7c208</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_FileMode</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>ac73c80b0e695d5bb64a1ee80469959db</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_FileSubstrate</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>ab0afb5c8e704d091b918c98e8209d43c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_FileType</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a71a0c92c58e88dc23a428cd10c4e3b92</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_FlushType</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a5047265362ee3b7639b5958981b1e0ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_GlobalDefReader_struct</type>
      <name>OTF2_GlobalDefReader</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a1de1e04c508979474f364346c33a2744</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_GlobalEvtReader_struct</type>
      <name>OTF2_GlobalEvtReader</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>af018087449879cb19f6f9b661430fbbc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_GlobalSnapReader_struct</type>
      <name>OTF2_GlobalSnapReader</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aef5c44d62cc7a725274428b7581dbb09</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_GroupRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a0c5626d2e81e6567483f555f0f0d72d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_LocationGroupRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a79d7c0e3dfb8ad8e92384d372a110bc3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>OTF2_LocationRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a22ad5564c5228e897ee40101c6f2dadc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_MappingType</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a13e3b6309325c0acb9d7acb3acf606a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_MarkerReader_struct</type>
      <name>OTF2_MarkerReader</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a52c8339bbe2b5bc3378aa0e13f6fbd74</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_MetricMemberRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a9a8b4bfd20fe6f72078301b8057138e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_MetricRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a43d4992e7cf140162588cdce25f255a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_Paradigm</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a554a465d6da0f96550b60f17c5bd8d09</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_ParameterRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>ac4f78b7fa5a47063f552259691c80e57</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_RegionRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>adfc139b4e2509d2dc3fb67f68586606b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_RmaWinRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>af4ea04325a75e552ebba308a622b533c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_SnapReader_struct</type>
      <name>OTF2_SnapReader</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a35c7eda9fd01ddee61149babd1654f52</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_StringRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a7244f0a8fe4bd0e29c9a236a9ba6c06a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_SystemTreeNodeRef</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a312145e6d3268cf99daf1d32358441fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_ThumbnailType</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>ac4b2ffa3e6c2af77ebe521af3532cc47</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>OTF2_TimeStamp</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>abda0922a9b24ae73817b19c2c6d1dd32</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_Type</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>afc875a8434ffb22290beacf55cd62b21</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_CallbackCode</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa2be133ee8253ed2695fd4cfc3008cbc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_CALLBACK_SUCCESS</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa2be133ee8253ed2695fd4cfc3008cbcaf6851257314713d3633b4e6f301d8f2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_CALLBACK_INTERRUPT</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa2be133ee8253ed2695fd4cfc3008cbca3ad90d6b7353bd07d8429b4ee5415ce4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_Compression_enum</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3ccd3ee3be247b05ebb875e37605c102</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_COMPRESSION_UNDEFINED</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3ccd3ee3be247b05ebb875e37605c102ac2387f05149b78fa1f9e5cfd4fdeac71</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_COMPRESSION_NONE</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3ccd3ee3be247b05ebb875e37605c102a51b61101bd0c9fffa6b1aac5c3c1486b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_COMPRESSION_ZLIB</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3ccd3ee3be247b05ebb875e37605c102ade1a5f120b0c16d51c89cbb181f586c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_FileMode_enum</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a80bc691fdbaedea3a1bae1891b691c70</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_FILEMODE_WRITE</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a80bc691fdbaedea3a1bae1891b691c70a51f8cd65bfee97aadaa7d864f04a75da</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_FILEMODE_READ</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a80bc691fdbaedea3a1bae1891b691c70a02fb33ca25bfcc48d9b7354c6c210f0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_FILEMODE_MODIFY</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a80bc691fdbaedea3a1bae1891b691c70a1559e8bb3b11efc6b56e6e75aedc4e8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_FileSubstrate_enum</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>abe8baeac8423cec94f9f7a3d0953df76</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SUBSTRATE_UNDEFINED</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>abe8baeac8423cec94f9f7a3d0953df76a538b841a7bdae8536c6b19faefe40bf1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SUBSTRATE_POSIX</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>abe8baeac8423cec94f9f7a3d0953df76a13e91f19c4da704ce087acc05ef8717f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SUBSTRATE_SION</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>abe8baeac8423cec94f9f7a3d0953df76aa4b16681a3e80e83aec402f6dba1a120</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SUBSTRATE_NONE</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>abe8baeac8423cec94f9f7a3d0953df76a8c66284ee5583c61793ae24b1651d814</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_FileType_enum</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a0fe7e27bfd56decbc72b104c85e6ca4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_FILETYPE_ANCHOR</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a0fe7e27bfd56decbc72b104c85e6ca4eae1a1102ec36ca3f54bde09c4203d2501</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_FILETYPE_GLOBAL_DEFS</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a0fe7e27bfd56decbc72b104c85e6ca4eaa67532a3daca76169abad7d12cf1e042</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_FILETYPE_LOCAL_DEFS</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a0fe7e27bfd56decbc72b104c85e6ca4eac6281cda76b5f4a65cb2c735fa11fe0d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_FILETYPE_EVENTS</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a0fe7e27bfd56decbc72b104c85e6ca4ea001ebb2cf528fca39de9e90bbda2929c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_FILETYPE_SNAPSHOTS</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a0fe7e27bfd56decbc72b104c85e6ca4eafbd8cc505e7f19296e7e045e7477c7c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_FILETYPE_THUMBNAIL</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a0fe7e27bfd56decbc72b104c85e6ca4ea474baf2fbb67d7c38321d692400a9834</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_FILETYPE_MARKER</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a0fe7e27bfd56decbc72b104c85e6ca4ea7b98e2b1d6e5b090ea41f15611bb2272</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_FlushType_enum</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a18d4d47b20f46be131d3d968f6f860ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_NO_FLUSH</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a18d4d47b20f46be131d3d968f6f860eaaf1cbff9080c71c8219cceaad64fb9dff</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_FLUSH</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a18d4d47b20f46be131d3d968f6f860eaa9ba2a59f01a892cff3c6ef4b13875acb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_MappingType_enum</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a06668669f0522a2e506f42f0bf8bcffe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MAPPING_STRING</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a06668669f0522a2e506f42f0bf8bcffea04495ee447e9b12bbe788a18e304fab9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MAPPING_ATTRIBUTE</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a06668669f0522a2e506f42f0bf8bcffeacbfddeffc1cf139140d0f50ae0b7ac03</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MAPPING_LOCATION</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a06668669f0522a2e506f42f0bf8bcffea2237a8cff445bb0878cb9dc9a130f46a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MAPPING_REGION</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a06668669f0522a2e506f42f0bf8bcffea0fe28515679433e7c7474f612bac0b65</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MAPPING_GROUP</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a06668669f0522a2e506f42f0bf8bcffeac9ca93380d29ea68e83165c597dfcc67</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MAPPING_METRIC</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a06668669f0522a2e506f42f0bf8bcffea357eb3fcb1814ef25510fabbb03a32ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MAPPING_COMM</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a06668669f0522a2e506f42f0bf8bcffea1ef93974e11a8668b981d9f0daf6c952</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MAPPING_PARAMETER</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a06668669f0522a2e506f42f0bf8bcffea3bf7aa91c2ca3f60b2010ef7aa7143c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MAPPING_RMA_WIN</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a06668669f0522a2e506f42f0bf8bcffeab51ceeebfb02a476e794fc54a6376cbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MAPPING_MAX</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a06668669f0522a2e506f42f0bf8bcffeaba1899676450f92fe8f42733d8b1d857</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_Paradigm_enum</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa14d0751354081d258913145a80e79a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_PARADIGM_UNKNOWN</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa14d0751354081d258913145a80e79a9a4fff0b0b311cd8b35f1bcf50a70613ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_PARADIGM_USER</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa14d0751354081d258913145a80e79a9a76784cbb6d6751820c8b760e80354a70</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_PARADIGM_COMPILER</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa14d0751354081d258913145a80e79a9a7d5900516ad5660c35683e5d2162de01</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_PARADIGM_OPENMP</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa14d0751354081d258913145a80e79a9aabdfda37f8c1f116655b1e3648bae137</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_PARADIGM_MPI</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa14d0751354081d258913145a80e79a9ae0b7a857a1c240cd1905210f76a64170</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_PARADIGM_CUDA</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa14d0751354081d258913145a80e79a9a9b11dc00f9ce1e4c55ad818ab0aa4808</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_PARADIGM_MEASUREMENT_SYSTEM</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aa14d0751354081d258913145a80e79a9a19db38d70290271755c6185474d77aa9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_ThumbnailType_enum</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aabc5719a3905b8f3c948f4d81044e556</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_THUMBNAIL_TYPE_REGION</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aabc5719a3905b8f3c948f4d81044e556adc03495ce36a43f03c3bc004cfd4f4f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_THUMBNAIL_TYPE_METRIC</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aabc5719a3905b8f3c948f4d81044e556a64956a2ce1b3b4b1a157f758d9e2d3d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_THUMBNAIL_TYPE_ATTRIBUTES</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>aabc5719a3905b8f3c948f4d81044e556a4abf2008629c9c1ac1a2509cb63b0b1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_Type_enum</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3d4247e27d833f7c2ef94929a7722002</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_TYPE_NONE</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3d4247e27d833f7c2ef94929a7722002a33f7135cd0e4f668f371d98770d4b800</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_TYPE_UINT8</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3d4247e27d833f7c2ef94929a7722002a98117855ff1adcff6d73e6bbdb2d82d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_TYPE_UINT16</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3d4247e27d833f7c2ef94929a7722002a85763951285c08aab9babcfd1cb51733</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_TYPE_UINT32</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3d4247e27d833f7c2ef94929a7722002a243fe45d4bbdb8f7ed1463747ff5eb5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_TYPE_UINT64</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3d4247e27d833f7c2ef94929a7722002aecf4084fbddb3354db9301e00e39d812</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_TYPE_INT8</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3d4247e27d833f7c2ef94929a7722002ac3bb6b29413d22881a3dcc66c6eaf29c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_TYPE_INT16</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3d4247e27d833f7c2ef94929a7722002af5100e8216f7ac5b3666c43373f815e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_TYPE_INT32</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3d4247e27d833f7c2ef94929a7722002a7630964b06d94c623d8dd5c74465b7ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_TYPE_INT64</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3d4247e27d833f7c2ef94929a7722002ab6b4b39098f78978150e91668e004d97</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_TYPE_FLOAT</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3d4247e27d833f7c2ef94929a7722002ab9f79b81ea3022e1ee340a49cbba9597</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_TYPE_DOUBLE</name>
      <anchorfile>OTF2__GeneralDefinitions_8h.html</anchorfile>
      <anchor>a3d4247e27d833f7c2ef94929a7722002a527093c39857ab462e9efc54b0252f37</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_GlobalDefReader.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__GlobalDefReader_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__Definitions_8h" name="OTF2_Definitions.h" local="no" imported="no">otf2/OTF2_Definitions.h</includes>
    <includes id="OTF2__GlobalDefReaderCallbacks_8h" name="OTF2_GlobalDefReaderCallbacks.h" local="no" imported="no">otf2/OTF2_GlobalDefReaderCallbacks.h</includes>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReader_ReadDefinitions</name>
      <anchorfile>OTF2__GlobalDefReader_8h.html</anchorfile>
      <anchor>a0a9ad0fe9c6f710fb2b1e9c1084f63cb</anchor>
      <arglist>(OTF2_GlobalDefReader *reader, uint64_t recordsToRead, uint64_t *recordsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReader_SetCallbacks</name>
      <anchorfile>OTF2__GlobalDefReader_8h.html</anchorfile>
      <anchor>a6f78c8ab0b228e76685d25d4172d211a</anchor>
      <arglist>(OTF2_GlobalDefReader *reader, const OTF2_GlobalDefReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_GlobalDefReaderCallbacks.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__GlobalDefReaderCallbacks_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__GeneralDefinitions_8h" name="OTF2_GeneralDefinitions.h" local="no" imported="no">otf2/OTF2_GeneralDefinitions.h</includes>
    <includes id="OTF2__Definitions_8h" name="OTF2_Definitions.h" local="no" imported="no">otf2/OTF2_Definitions.h</includes>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_Attribute</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>aa9f6b692110612249c256d0d8e16af41</anchor>
      <arglist>)(void *userData, OTF2_AttributeRef self, OTF2_StringRef name, OTF2_Type type)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_Callpath</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>ad31e71da1a68f1727307cd498661051f</anchor>
      <arglist>)(void *userData, OTF2_CallpathRef self, OTF2_CallpathRef parent, OTF2_RegionRef region)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_Callsite</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a281a1de48a8e1f08ee3bc08e76c296f6</anchor>
      <arglist>)(void *userData, OTF2_CallsiteRef self, OTF2_StringRef sourceFile, uint32_t lineNumber, OTF2_RegionRef enteredRegion, OTF2_RegionRef leftRegion)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_ClockProperties</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a1c8bc1788e298a97ea3be27194c69a5c</anchor>
      <arglist>)(void *userData, uint64_t timerResolution, uint64_t globalOffset, uint64_t traceLength)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_Comm</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>abf489adc72880e0ed996a0363818d8a7</anchor>
      <arglist>)(void *userData, OTF2_CommRef self, OTF2_StringRef name, OTF2_GroupRef group, OTF2_CommRef parent)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_Group</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a73bb8b5c94ba113b5fc0f35760bb31ac</anchor>
      <arglist>)(void *userData, OTF2_GroupRef self, OTF2_StringRef name, OTF2_GroupType groupType, OTF2_Paradigm paradigm, OTF2_GroupFlag groupFlags, uint32_t numberOfMembers, const uint64_t *members)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_Location</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a6bf17a0e07e2222f113ef8c7576504e2</anchor>
      <arglist>)(void *userData, OTF2_LocationRef self, OTF2_StringRef name, OTF2_LocationType locationType, uint64_t numberOfEvents, OTF2_LocationGroupRef locationGroup)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_LocationGroup</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>adbfab409bfa2557221d3d96329a82365</anchor>
      <arglist>)(void *userData, OTF2_LocationGroupRef self, OTF2_StringRef name, OTF2_LocationGroupType locationGroupType, OTF2_SystemTreeNodeRef systemTreeParent)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_MetricClass</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a34df2d7a8f428351df2bb61ee62fbc9a</anchor>
      <arglist>)(void *userData, OTF2_MetricRef self, uint8_t numberOfMetrics, const OTF2_MetricMemberRef *metricMembers, OTF2_MetricOccurrence metricOccurrence, OTF2_RecorderKind recorderKind)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_MetricClassRecorder</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a89cd6185921067fdc32ca7f41c16b79f</anchor>
      <arglist>)(void *userData, OTF2_MetricRef metricClass, OTF2_LocationRef recorder)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_MetricInstance</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>ae21dca15aaafc130acf2b9df4d8e202f</anchor>
      <arglist>)(void *userData, OTF2_MetricRef self, OTF2_MetricRef metricClass, OTF2_LocationRef recorder, OTF2_MetricScope metricScope, uint64_t scope)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_MetricMember</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>af5471cc614db8e1d90b143e90ca71c1c</anchor>
      <arglist>)(void *userData, OTF2_MetricMemberRef self, OTF2_StringRef name, OTF2_StringRef description, OTF2_MetricType metricType, OTF2_MetricMode metricMode, OTF2_Type valueType, OTF2_MetricBase metricBase, int64_t exponent, OTF2_StringRef unit)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_Parameter</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a1e4cec1363663505059337646fadc1ec</anchor>
      <arglist>)(void *userData, OTF2_ParameterRef self, OTF2_StringRef name, OTF2_ParameterType parameterType)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_Region</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a4de6ac105f3e03f84bd4869414a46301</anchor>
      <arglist>)(void *userData, OTF2_RegionRef self, OTF2_StringRef name, OTF2_StringRef canonicalName, OTF2_StringRef description, OTF2_RegionRole regionRole, OTF2_Paradigm paradigm, OTF2_RegionFlag regionFlags, OTF2_StringRef sourceFile, uint32_t beginLineNumber, uint32_t endLineNumber)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_RmaWin</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>ab8a4ad419aac98bb94700cc368f7f65c</anchor>
      <arglist>)(void *userData, OTF2_RmaWinRef self, OTF2_StringRef name, OTF2_CommRef comm)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_String</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a1aa2a6b1f91ed864869d3eea1ac7c90f</anchor>
      <arglist>)(void *userData, OTF2_StringRef self, const char *string)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_SystemTreeNode</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a23452129180b7d6ed4fbf73b2fce80ec</anchor>
      <arglist>)(void *userData, OTF2_SystemTreeNodeRef self, OTF2_StringRef name, OTF2_StringRef className, OTF2_SystemTreeNodeRef parent)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_SystemTreeNodeDomain</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>aa9b01e94b5ec0a0c9cf3b1e7c01b2d09</anchor>
      <arglist>)(void *userData, OTF2_SystemTreeNodeRef systemTreeNode, OTF2_SystemTreeDomain systemTreeDomain)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_SystemTreeNodeProperty</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>ab7c6746bcdcd83738b98bf80b851c612</anchor>
      <arglist>)(void *userData, OTF2_SystemTreeNodeRef systemTreeNode, OTF2_StringRef name, OTF2_StringRef value)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalDefReaderCallback_Unknown</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>ae52b3db33e9cd38916b59731e51d5c62</anchor>
      <arglist>)(void *userData)</arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_GlobalDefReaderCallbacks_struct</type>
      <name>OTF2_GlobalDefReaderCallbacks</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a3cebfef59382a41a8584fa95b68d89da</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_GlobalDefReaderCallbacks_Clear</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>ae4b5871faba9ceb94d44b8b8dfef2134</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_GlobalDefReaderCallbacks_Delete</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a0b7640c20d7df47c024b6e45a52acecc</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_GlobalDefReaderCallbacks *</type>
      <name>OTF2_GlobalDefReaderCallbacks_New</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>ac21ba9d05e38dd8e705e1b74f5b21820</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetAttributeCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>aa370959333bfe1f395c2d00cbc5d52d6</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_Attribute attributeCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetCallpathCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a9552ce50eb2032892c667b1f7268feca</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_Callpath callpathCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetCallsiteCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a7c77faf2a18cc6463e06f336b0b079c4</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_Callsite callsiteCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetClockPropertiesCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a0867b4a6f1b42589eebab231bc834892</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_ClockProperties clockPropertiesCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetCommCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>adfcdc0efe50ef24b80122da36476f22d</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_Comm commCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetGroupCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a879cd6e2fea843b9fdf932c81ac6096d</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_Group groupCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetLocationCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>afeed571e0e4b35e54b220360df979d5a</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_Location locationCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetLocationGroupCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>aab4f4ba9bc46f00b1975422968f0f04f</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_LocationGroup locationGroupCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetMetricClassCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a7190bca51466fd3d7e53e6e9ba9e4471</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_MetricClass metricClassCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetMetricClassRecorderCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a736c46892748407fdb695c6e4d254ac6</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_MetricClassRecorder metricClassRecorderCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetMetricInstanceCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a548c24f355736de1c5bc7514d528156e</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_MetricInstance metricInstanceCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetMetricMemberCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>afd2965fef59af45e7c674bbee87d2bd6</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_MetricMember metricMemberCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetParameterCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a0752e1ce0cea1bbdcb3cdae7eada1c46</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_Parameter parameterCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetRegionCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a950e04b38ae8598ce789f930d1786961</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_Region regionCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetRmaWinCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a94fba3f74eba152b104f1431f409244b</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_RmaWin rmaWinCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetStringCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>ad0945fc077972d0f3a92e21430fc86e1</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_String stringCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodeCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a164272aac9d6a96313e45e18bee1395f</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_SystemTreeNode systemTreeNodeCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodeDomainCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a847eabf60034e21ed43c2aea0c6952c4</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_SystemTreeNodeDomain systemTreeNodeDomainCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodePropertyCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>ab7e1601086c51b7d4617e66a2f0fa014</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_SystemTreeNodeProperty systemTreeNodePropertyCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefReaderCallbacks_SetUnknownCallback</name>
      <anchorfile>OTF2__GlobalDefReaderCallbacks_8h.html</anchorfile>
      <anchor>a2cd75459adbae77a394ba9d1f8819255</anchor>
      <arglist>(OTF2_GlobalDefReaderCallbacks *globalDefReaderCallbacks, OTF2_GlobalDefReaderCallback_Unknown unknownCallback)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_GlobalDefWriter.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__GlobalDefWriter_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__Definitions_8h" name="OTF2_Definitions.h" local="no" imported="no">otf2/OTF2_Definitions.h</includes>
    <member kind="typedef">
      <type>struct OTF2_GlobalDefWriter_struct</type>
      <name>OTF2_GlobalDefWriter</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a02d4c04e1b1880e83c7d156b6531c2e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_GetNumberOfDefinitions</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a944627cc48cf7ed2a5f2d053d284899f</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, uint64_t *numberOfDefinitions)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_GetNumberOfLocations</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>acbe399ca68225b7fb5dc40213f518956</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, uint64_t *numberOfLocations)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteAttribute</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a0c66d0a754e7f5de20190e2a832de175</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_AttributeRef self, OTF2_StringRef name, OTF2_Type type)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteCallpath</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a2733a12e7a305b0eb4a768d9b091a06f</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_CallpathRef self, OTF2_CallpathRef parent, OTF2_RegionRef region)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteCallsite</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a205c2860bdc828c2c0b021cc61cf5585</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_CallsiteRef self, OTF2_StringRef sourceFile, uint32_t lineNumber, OTF2_RegionRef enteredRegion, OTF2_RegionRef leftRegion)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteClockProperties</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a569258568915237ccb049a0b35fe0592</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, uint64_t timerResolution, uint64_t globalOffset, uint64_t traceLength)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteComm</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>ac4c5980f20092b5a1f9965b5f2645d93</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_CommRef self, OTF2_StringRef name, OTF2_GroupRef group, OTF2_CommRef parent)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteGroup</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>acdbf890f38aa53aae20a478a9e135def</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_GroupRef self, OTF2_StringRef name, OTF2_GroupType groupType, OTF2_Paradigm paradigm, OTF2_GroupFlag groupFlags, uint32_t numberOfMembers, const uint64_t *members)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteLocation</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>aa173232da3f0312fbadf0da04b4008cb</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_LocationRef self, OTF2_StringRef name, OTF2_LocationType locationType, uint64_t numberOfEvents, OTF2_LocationGroupRef locationGroup)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteLocationGroup</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a56c0076b5d798a5b4658e85fab2c013e</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_LocationGroupRef self, OTF2_StringRef name, OTF2_LocationGroupType locationGroupType, OTF2_SystemTreeNodeRef systemTreeParent)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteMetricClass</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a6a282167f60768fe1dce0adf5f393b6d</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_MetricRef self, uint8_t numberOfMetrics, const OTF2_MetricMemberRef *metricMembers, OTF2_MetricOccurrence metricOccurrence, OTF2_RecorderKind recorderKind)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteMetricClassRecorder</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a0d6ac01f3ec1ff9f286ba3a6e85bc82a</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_MetricRef metricClass, OTF2_LocationRef recorder)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteMetricInstance</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a92a8b935b77cd6bdc86cd23efbc9dda7</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_MetricRef self, OTF2_MetricRef metricClass, OTF2_LocationRef recorder, OTF2_MetricScope metricScope, uint64_t scope)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteMetricMember</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a80e6fb6a7186527f24e47beeb09fd485</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_MetricMemberRef self, OTF2_StringRef name, OTF2_StringRef description, OTF2_MetricType metricType, OTF2_MetricMode metricMode, OTF2_Type valueType, OTF2_MetricBase metricBase, int64_t exponent, OTF2_StringRef unit)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteParameter</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>aeb8b8d22cb721ef0ea6641c5ed19588c</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_ParameterRef self, OTF2_StringRef name, OTF2_ParameterType parameterType)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteRegion</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>ac44eba6a61321aa75a310e022bfb1ae4</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_RegionRef self, OTF2_StringRef name, OTF2_StringRef canonicalName, OTF2_StringRef description, OTF2_RegionRole regionRole, OTF2_Paradigm paradigm, OTF2_RegionFlag regionFlags, OTF2_StringRef sourceFile, uint32_t beginLineNumber, uint32_t endLineNumber)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteRmaWin</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a98b500522f8c128c4d3c396c728af27c</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_RmaWinRef self, OTF2_StringRef name, OTF2_CommRef comm)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteString</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a25699acbc67d818fb395aeb91a993d57</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_StringRef self, const char *string)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteSystemTreeNode</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a7769fd30e2c3493f35e20bef29838326</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_SystemTreeNodeRef self, OTF2_StringRef name, OTF2_StringRef className, OTF2_SystemTreeNodeRef parent)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteSystemTreeNodeDomain</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>a787c8f51fd3cae79c2eb529b7482698a</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_SystemTreeNodeRef systemTreeNode, OTF2_SystemTreeDomain systemTreeDomain)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalDefWriter_WriteSystemTreeNodeProperty</name>
      <anchorfile>OTF2__GlobalDefWriter_8h.html</anchorfile>
      <anchor>aadb1fa035c01d28b1f492dccc1ffe61a</anchor>
      <arglist>(OTF2_GlobalDefWriter *writerHandle, OTF2_SystemTreeNodeRef systemTreeNode, OTF2_StringRef name, OTF2_StringRef value)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_GlobalEvtReader.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__GlobalEvtReader_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__EvtReader_8h" name="OTF2_EvtReader.h" local="no" imported="no">otf2/OTF2_EvtReader.h</includes>
    <includes id="OTF2__GlobalEvtReaderCallbacks_8h" name="OTF2_GlobalEvtReaderCallbacks.h" local="no" imported="no">otf2/OTF2_GlobalEvtReaderCallbacks.h</includes>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReader_HasEvent</name>
      <anchorfile>OTF2__GlobalEvtReader_8h.html</anchorfile>
      <anchor>a22d68bbcaed6da558e1d61a0d9278b89</anchor>
      <arglist>(OTF2_GlobalEvtReader *reader, int *flag)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReader_ReadEvent</name>
      <anchorfile>OTF2__GlobalEvtReader_8h.html</anchorfile>
      <anchor>ad217be41434c56cb9ca54cd6ae377c71</anchor>
      <arglist>(OTF2_GlobalEvtReader *reader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReader_ReadEvents</name>
      <anchorfile>OTF2__GlobalEvtReader_8h.html</anchorfile>
      <anchor>aeffc3e12c5b8e31c0c40bfac8959be97</anchor>
      <arglist>(OTF2_GlobalEvtReader *reader, uint64_t recordsToRead, uint64_t *recordsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReader_SetCallbacks</name>
      <anchorfile>OTF2__GlobalEvtReader_8h.html</anchorfile>
      <anchor>a34386d8ac80d988c3ad43c9ceaedefcb</anchor>
      <arglist>(OTF2_GlobalEvtReader *reader, const OTF2_GlobalEvtReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_GlobalEvtReaderCallbacks.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__GlobalEvtReaderCallbacks_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__GeneralDefinitions_8h" name="OTF2_GeneralDefinitions.h" local="no" imported="no">otf2/OTF2_GeneralDefinitions.h</includes>
    <includes id="OTF2__AttributeList_8h" name="OTF2_AttributeList.h" local="no" imported="no">otf2/OTF2_AttributeList.h</includes>
    <includes id="OTF2__Events_8h" name="OTF2_Events.h" local="no" imported="no">otf2/OTF2_Events.h</includes>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_BufferFlush</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>af898c5a3f5070e5b69b1a1cd203699fd</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp stopTime)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_Enter</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aef5186918ac220d2837e19f2591fa767</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RegionRef region)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_Leave</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a20ff06fbea1a31831f263a3dbd9755a5</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RegionRef region)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_MeasurementOnOff</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a88d1a08c88a883f7e67f1edc6432afa5</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_MeasurementMode measurementMode)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_Metric</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a8d168170e3e3989442d3b75abf09e120</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_MetricRef metric, uint8_t numberOfMetrics, const OTF2_Type *typeIDs, const OTF2_MetricValue *metricValues)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_MpiCollectiveBegin</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a3a9047639db4c8c7a8ef1d25da0dbeea</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_MpiCollectiveEnd</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a6f10011509a6873242ab0df178b7bcb7</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_CollectiveOp collectiveOp, OTF2_CommRef communicator, uint32_t root, uint64_t sizeSent, uint64_t sizeReceived)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_MpiIrecv</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>af750d824d15c631281666c613235acd1</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint32_t sender, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_MpiIrecvRequest</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a2cb70f5051d1baa54887f2a6f11c70ea</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_MpiIsend</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a4aa160c52c56ec17ed91d1b3dae2933b</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint32_t receiver, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_MpiIsendComplete</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a49a7af52274fc5e00f24b0ef5a35b4a8</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_MpiRecv</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a3ef354957e9a21f8348635b913a6d2c9</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint32_t sender, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_MpiRequestCancelled</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a84fddaf4edd42c36dccf80e1b18ca138</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_MpiRequestTest</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a7f8777e3a88cf1cb6657e6458244b9ea</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_MpiSend</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>af994ed7826d5bb018a397ec31ad46845</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint32_t receiver, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_OmpAcquireLock</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a22927a9598573fd3f3870a00fa6ed6ff</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_OmpFork</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a859cfdc0ebf0522016e99aeb4295ce27</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint32_t numberOfRequestedThreads)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_OmpJoin</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ae7a7be8c08f48ae3cf508d9552c2df52</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_OmpReleaseLock</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a16f6886165bd428a9069cc480cca06e8</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_OmpTaskComplete</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a981480c2718e9ae0a308d1548e2ddbbf</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint64_t taskID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_OmpTaskCreate</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a7aeaef5663c2728f771bc14489fb5b00</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint64_t taskID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_OmpTaskSwitch</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aeb5b6cf66c955469f1fc938b519eb1ec</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, uint64_t taskID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_ParameterInt</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a42565ea2cacf148f7b9856b5af745904</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_ParameterRef parameter, int64_t value)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_ParameterString</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a6bc4b50e3cb14806b6b7bc6f55417a66</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_ParameterRef parameter, OTF2_StringRef string)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_ParameterUnsignedInt</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a41805c8f1d10a7f363aaed1d95779c2e</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_ParameterRef parameter, uint64_t value)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaAcquireLock</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a35118a9c393d54ff052a75929761b84b</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, uint64_t lockId, OTF2_LockType lockType)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaAtomic</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a64e6100c2f8d249af93be89238d5217d</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, OTF2_RmaAtomicType type, uint64_t bytesSent, uint64_t bytesReceived, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaCollectiveBegin</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a972841b000fd9e7c074eeb4e7925b798</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaCollectiveEnd</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ac08243743dc68d83fbffff6d18b75a31</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_CollectiveOp collectiveOp, OTF2_RmaSyncLevel syncLevel, OTF2_RmaWinRef win, uint32_t root, uint64_t bytesSent, uint64_t bytesReceived)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaGet</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ae31fdf54d907be18d58f57f1b4a90bb1</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, uint64_t bytes, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaGroupSync</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a957260158256874d0f8619cb4b88224c</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaSyncLevel syncLevel, OTF2_RmaWinRef win, OTF2_GroupRef group)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaOpCompleteBlocking</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a12a65783baf8241fa372c8c50a8a8afa</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaOpCompleteNonBlocking</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a2e430366f35eee588bbff887e74c5837</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaOpCompleteRemote</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a754c43c7a47588202879aff426f43ad3</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaOpTest</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a9731d0915784950b173c0c04dfe3f397</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaPut</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a8bf3787801054c228eb1452049defe5a</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, uint64_t bytes, uint64_t matchingId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaReleaseLock</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a692c9f38272a8556ae9a7dc5a0b27549</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, uint64_t lockId)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaRequestLock</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a9a3d4e86e20ef056ff4c07771a1428c7</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, uint64_t lockId, OTF2_LockType lockType)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaSync</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a2553f3d41b5a08f5fc628850e3773c1b</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, OTF2_RmaSyncType syncType)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaTryLock</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a29e02bf3929084c1290a3b529e9850ea</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win, uint32_t remote, uint64_t lockId, OTF2_LockType lockType)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaWaitChange</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>af7f2e1d674aa7ee6fbdc5cc83f329b64</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaWinCreate</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a198b743053d4a3f6336bbebffc3475e6</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_RmaWinDestroy</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>abf3ec0a4f29faf453720fbeb578bb1d0</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_RmaWinRef win)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_ThreadAcquireLock</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a6aa397a352bf7bd17271b1cef6197c41</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_Paradigm model, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_ThreadFork</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ac21f9ae282a3236f199f87764d1d5060</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_Paradigm model, uint32_t numberOfRequestedThreads)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_ThreadJoin</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a16f72440f82e337865003885702f683f</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_Paradigm model)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_ThreadReleaseLock</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a608d7b5a837ecdd26925012537981c20</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_Paradigm model, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_ThreadTaskComplete</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a1870d5e86e68320d78005d61766d7527</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_CommRef threadTeam, uint32_t creatingThread, uint32_t generationNumber)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_ThreadTaskCreate</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a750ba49618ef52f323fc008a576b2e1a</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_CommRef threadTeam, uint32_t creatingThread, uint32_t generationNumber)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_ThreadTaskSwitch</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a12b347209e8f30ce6ea7a1f8b7242eff</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_CommRef threadTeam, uint32_t creatingThread, uint32_t generationNumber)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_ThreadTeamBegin</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a0034fe635c2daf517278caa767638d06</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_CommRef threadTeam)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_ThreadTeamEnd</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ad649ddd264ffb7a5691b74942366d807</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList, OTF2_CommRef threadTeam)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalEvtReaderCallback_Unknown</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a21095abb8bdcaf785a9a272bf804adbb</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp time, void *userData, OTF2_AttributeList *attributeList)</arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_GlobalEvtReaderCallbacks_struct</type>
      <name>OTF2_GlobalEvtReaderCallbacks</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>af852799318a4c41e25b8113c4e77f52a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_GlobalEvtReaderCallbacks_Clear</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a0b6403cf0bae70c4012adb875c8913f1</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_GlobalEvtReaderCallbacks_Delete</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a33b8c54ca27bd4a4325965101c8ea4e9</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_GlobalEvtReaderCallbacks *</type>
      <name>OTF2_GlobalEvtReaderCallbacks_New</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a96bedd27d78efa72ce1747b6bf0319bc</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetBufferFlushCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a104abf0923c63f2dd1aede5c6bfbb232</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_BufferFlush bufferFlushCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetEnterCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ad666bbeb47c97ef3b54e18cea6ae0c8f</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_Enter enterCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetLeaveCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a1d3e114481114355634cff54b04d67ce</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_Leave leaveCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetMeasurementOnOffCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a66b69ca4d18e950df125ceaab4a1718f</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_MeasurementOnOff measurementOnOffCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetMetricCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aa0aeee7755c1b4fd7fe45168695256de</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_Metric metricCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetMpiCollectiveBeginCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a6abc324a0f8b7cd50aa206474b928448</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_MpiCollectiveBegin mpiCollectiveBeginCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetMpiCollectiveEndCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a943d5c753143924af001fbc6efd3d73f</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_MpiCollectiveEnd mpiCollectiveEndCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetMpiIrecvCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>afce33d1aac917fbc1d0b6c4ae680de1d</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_MpiIrecv mpiIrecvCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetMpiIrecvRequestCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a55669129534c2f0ee2b3152e415cad0e</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_MpiIrecvRequest mpiIrecvRequestCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetMpiIsendCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a2e83a27358adde54bcd0a2d558008b2e</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_MpiIsend mpiIsendCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetMpiIsendCompleteCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aa03fbe79959d1600dc146a34ab5ed7bb</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_MpiIsendComplete mpiIsendCompleteCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetMpiRecvCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a5db439eb57e5e7fd6511734afda5938a</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_MpiRecv mpiRecvCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetMpiRequestCancelledCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aff3a49e4f757aca0cdfd8a6d6b9d5ec9</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_MpiRequestCancelled mpiRequestCancelledCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetMpiRequestTestCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ac0454312dba2a593eae6ea371cd11859</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_MpiRequestTest mpiRequestTestCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetMpiSendCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a38e9bc7fab5281af52ad8174689abdae</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_MpiSend mpiSendCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetOmpAcquireLockCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a1de619201b55508fa286644d37a059af</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_OmpAcquireLock ompAcquireLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetOmpForkCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ae8220adaaae76b247b9b6de736023eaf</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_OmpFork ompForkCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetOmpJoinCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ab12d312d9755830b904ae904c373a71a</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_OmpJoin ompJoinCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetOmpReleaseLockCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a183495daf329b2f3d999c887cbd93582</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_OmpReleaseLock ompReleaseLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetOmpTaskCompleteCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>acc0f8f43c9187ee6f18a894ad94d4311</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_OmpTaskComplete ompTaskCompleteCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetOmpTaskCreateCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ad25db1d0387df1c8085208295b8c484e</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_OmpTaskCreate ompTaskCreateCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetOmpTaskSwitchCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a5a06a5b332da5e1b635542d65b102ffd</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_OmpTaskSwitch ompTaskSwitchCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetParameterIntCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a074c9dcee4c3f8589b659d0e5a58ddd4</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_ParameterInt parameterIntCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetParameterStringCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aa7d37ec6501dca04dd1fb08ec7051fd0</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_ParameterString parameterStringCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetParameterUnsignedIntCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a2107a82e48a047507255c2b850dd5e9f</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_ParameterUnsignedInt parameterUnsignedIntCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaAcquireLockCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a0688ec5c38004a231b78e5a2701f0652</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaAcquireLock rmaAcquireLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaAtomicCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aaa70e5e9370057a6d25e7218f50bfb94</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaAtomic rmaAtomicCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaCollectiveBeginCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a3477743c7daa6b09df922a5e8043af8a</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaCollectiveBegin rmaCollectiveBeginCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaCollectiveEndCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>abd618b576e1c0f5511ad3bd38be34fae</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaCollectiveEnd rmaCollectiveEndCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaGetCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ad18aa8b588055be39255a5db77d006ca</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaGet rmaGetCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaGroupSyncCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aecef2b20d0550763b7095e8bab447c0a</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaGroupSync rmaGroupSyncCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaOpCompleteBlockingCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a3b1c3f2d7524faebc4c98a9c5389c835</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaOpCompleteBlocking rmaOpCompleteBlockingCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaOpCompleteNonBlockingCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a9f2486bcb3b29f3db63edc74c40c84e2</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaOpCompleteNonBlocking rmaOpCompleteNonBlockingCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaOpCompleteRemoteCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a487b977996bd2120d3f7549449dcbf6d</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaOpCompleteRemote rmaOpCompleteRemoteCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaOpTestCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aa00efc74e4499ff151315cc29abe4a3f</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaOpTest rmaOpTestCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaPutCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a0114fccb5e0248ea630008f42334f140</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaPut rmaPutCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaReleaseLockCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ac238d5cb527adfab9ae6b2508492e6a3</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaReleaseLock rmaReleaseLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaRequestLockCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>aa6b280432a8a082b25f72d7633ac1076</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaRequestLock rmaRequestLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaSyncCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a79e027f3ede9ffdebad73dcbc3213645</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaSync rmaSyncCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaTryLockCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ae746d98163e7696835ef5ceb763c3892</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaTryLock rmaTryLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaWaitChangeCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a7c3cc1570577ffed40c8291b29911681</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaWaitChange rmaWaitChangeCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaWinCreateCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a61e8c27a99aeb0ee488a3c0a7353a851</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaWinCreate rmaWinCreateCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetRmaWinDestroyCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a258ebd4aa93c8e257d39c7eb21b3c0bc</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_RmaWinDestroy rmaWinDestroyCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetThreadAcquireLockCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a65aeea1df5afcf3d192d3d89feedfbe6</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_ThreadAcquireLock threadAcquireLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetThreadForkCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ad38de3ac4e220a91f849322b1a7a748c</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_ThreadFork threadForkCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetThreadJoinCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>ab26d0711289fdfb2640a622a09ac59de</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_ThreadJoin threadJoinCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetThreadReleaseLockCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a36b3d2f075d5a5317832c8680a55f6c4</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_ThreadReleaseLock threadReleaseLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetThreadTaskCompleteCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a23a2fb04efc7c4ee2b510d9d70e50914</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_ThreadTaskComplete threadTaskCompleteCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetThreadTaskCreateCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a1b722c2f0cb9371250fce244ddfa265f</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_ThreadTaskCreate threadTaskCreateCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetThreadTaskSwitchCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a91038d5bd1e896b13042ccf1f5854752</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_ThreadTaskSwitch threadTaskSwitchCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetThreadTeamBeginCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a357f5b96d487e82924a2280b7207bcbc</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_ThreadTeamBegin threadTeamBeginCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetThreadTeamEndCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a1abb1ed97ff8d7e1acad4b0d1a7ec47d</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_ThreadTeamEnd threadTeamEndCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalEvtReaderCallbacks_SetUnknownCallback</name>
      <anchorfile>OTF2__GlobalEvtReaderCallbacks_8h.html</anchorfile>
      <anchor>a5e9cfba9b50b901757c464a29d52a351</anchor>
      <arglist>(OTF2_GlobalEvtReaderCallbacks *globalEvtReaderCallbacks, OTF2_GlobalEvtReaderCallback_Unknown unknownCallback)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_GlobalSnapReader.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__GlobalSnapReader_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__SnapReader_8h" name="OTF2_SnapReader.h" local="no" imported="no">otf2/OTF2_SnapReader.h</includes>
    <includes id="OTF2__GlobalSnapReaderCallbacks_8h" name="OTF2_GlobalSnapReaderCallbacks.h" local="no" imported="no">otf2/OTF2_GlobalSnapReaderCallbacks.h</includes>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReader_ReadSnapshots</name>
      <anchorfile>OTF2__GlobalSnapReader_8h.html</anchorfile>
      <anchor>ac2e9ba47e6822a9b5035d8fd12b45e68</anchor>
      <arglist>(OTF2_GlobalSnapReader *reader, uint64_t recordsToRead, uint64_t *recordsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReader_SetCallbacks</name>
      <anchorfile>OTF2__GlobalSnapReader_8h.html</anchorfile>
      <anchor>a6b577ecab35fa98150264734322b3328</anchor>
      <arglist>(OTF2_GlobalSnapReader *reader, const OTF2_GlobalSnapReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_GlobalSnapReaderCallbacks.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__GlobalSnapReaderCallbacks_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__GeneralDefinitions_8h" name="OTF2_GeneralDefinitions.h" local="no" imported="no">otf2/OTF2_GeneralDefinitions.h</includes>
    <includes id="OTF2__AttributeList_8h" name="OTF2_AttributeList.h" local="no" imported="no">otf2/OTF2_AttributeList.h</includes>
    <includes id="OTF2__Events_8h" name="OTF2_Events.h" local="no" imported="no">otf2/OTF2_Events.h</includes>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_Enter</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a42bee06ccf348ef0471888201a5aea45</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_RegionRef region)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_MeasurementOnOff</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a84eb4675c67649f9ee8973dcc89089ff</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_MeasurementMode measurementMode)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_Metric</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a3ef0f62b94a2bbb4aa4b78720e7a43d1</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_MetricRef metric, uint8_t numberOfMetrics, const OTF2_Type *typeIDs, const OTF2_MetricValue *metricValues)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_MpiCollectiveBegin</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>af5d51e49a4555ae955f28fc5c882d8d1</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_MpiCollectiveEnd</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a903dee3672b7dc351f219bc83486b835</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_CollectiveOp collectiveOp, OTF2_CommRef communicator, uint32_t root, uint64_t sizeSent, uint64_t sizeReceived)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_MpiIrecv</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a4b17302e0b859299d112e78e2443a108</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint32_t sender, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_MpiIrecvRequest</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a1b767cf0cd6b6b241d7b7a60bce78576</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_MpiIsend</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a685618713e30aaf5d5466ddfea54cdb5</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint32_t receiver, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_MpiIsendComplete</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a45fd72023c197e7b1f12600c40423c44</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_MpiRecv</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>ace0048ac98e2ca55d9540416ad690426</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint32_t sender, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_MpiSend</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a54e66604ea7ddf2006c4dde6e6efe445</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint32_t receiver, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_OmpAcquireLock</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a8fc1ba792ae43f78756d9ea5e63aae8f</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_OmpFork</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a480faf8713ab899dd2f2ae382177e4b8</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint32_t numberOfRequestedThreads)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_OmpTaskCreate</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a0a745c2dae5e3d7210bc3c1461e34191</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint64_t taskID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_OmpTaskSwitch</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>aba53afb458abbd497af533ecc29a84c0</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint64_t taskID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_ParameterInt</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>aa1c542faea41c6ba0589e144f29df914</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_ParameterRef parameter, int64_t value)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_ParameterString</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>aba6d266686b794d50fa257745d4dd040</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_ParameterRef parameter, OTF2_StringRef string)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_ParameterUnsignedInt</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a97ee76d64cdfec41fd8245fca65556a0</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_ParameterRef parameter, uint64_t value)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_SnapshotEnd</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a4a781f4bbb1a774b4659179a8b3eec1b</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, uint64_t contReadPos)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_SnapshotStart</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a6b25b6681a0496fdcd473eb511b7eb86</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, uint64_t numberOfRecords)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_GlobalSnapReaderCallback_Unknown</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a92766c0f88ea746d2e8245549350fec4</anchor>
      <arglist>)(OTF2_LocationRef locationID, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList)</arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_GlobalSnapReaderCallbacks_struct</type>
      <name>OTF2_GlobalSnapReaderCallbacks</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>afe8efbf85825b05b95246363eeeadff6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_GlobalSnapReaderCallbacks_Clear</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a43cce1c432428d8afe45e257052614a6</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_GlobalSnapReaderCallbacks_Delete</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a8869ed44ca3433a09c4345b0cdfbbf96</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_GlobalSnapReaderCallbacks *</type>
      <name>OTF2_GlobalSnapReaderCallbacks_New</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a0e940ab8211da6b7c8b46a968cb0c637</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetEnterCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a70a7abf6ab12260cb23789fd66f77cda</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_Enter enterCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetMeasurementOnOffCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a5ec524732243555d01edb6db3a5b4788</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_MeasurementOnOff measurementOnOffCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetMetricCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a8545805374c8bf9fa4edb28e59c0491a</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_Metric metricCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetMpiCollectiveBeginCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>af7a4020aded8ef931d2ac9adea1ddde6</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_MpiCollectiveBegin mpiCollectiveBeginCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetMpiCollectiveEndCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>ade3e26c720ce316f93f9ee72818d3644</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_MpiCollectiveEnd mpiCollectiveEndCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetMpiIrecvCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>ae5dcc27161ea3fc41c8899d9fb2c4610</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_MpiIrecv mpiIrecvCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetMpiIrecvRequestCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a1129b3a61e5cd6e49514a36b0db24f36</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_MpiIrecvRequest mpiIrecvRequestCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetMpiIsendCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a046c47b76133a05c1b37a3d71f784162</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_MpiIsend mpiIsendCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetMpiIsendCompleteCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a8b64fa756e15565af23fba3ad741c749</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_MpiIsendComplete mpiIsendCompleteCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetMpiRecvCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>afc8ec8252157882ec1c96f634255b82b</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_MpiRecv mpiRecvCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetMpiSendCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a2a5023b0fbf8ecf9ce9ea742d999615e</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_MpiSend mpiSendCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetOmpAcquireLockCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a0f1cf4d41602f718d18f64f0f140b52a</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_OmpAcquireLock ompAcquireLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetOmpForkCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a296b959db5fb16845a9262e2c2cf0f35</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_OmpFork ompForkCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetOmpTaskCreateCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a792cbbe6d8f6e6d822119e75c45fdb43</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_OmpTaskCreate ompTaskCreateCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetOmpTaskSwitchCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a5873744f8878b1b122ea9f089497dbb3</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_OmpTaskSwitch ompTaskSwitchCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetParameterIntCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a91477600ededcfb3b3931694378eaea0</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_ParameterInt parameterIntCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetParameterStringCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>ad9a260cc715f8b34d57e3facdef34ca6</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_ParameterString parameterStringCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetParameterUnsignedIntCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a6c52f16235e114a59103cfcd6bb75cfd</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_ParameterUnsignedInt parameterUnsignedIntCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetSnapshotEndCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a4b787daf916331f5800cc6d23d008b7a</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_SnapshotEnd snapshotEndCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetSnapshotStartCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a3710c95dc75a2e938d0fb3c56eae4d92</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_SnapshotStart snapshotStartCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_GlobalSnapReaderCallbacks_SetUnknownCallback</name>
      <anchorfile>OTF2__GlobalSnapReaderCallbacks_8h.html</anchorfile>
      <anchor>aa9e6027f57654546101493013b424078</anchor>
      <arglist>(OTF2_GlobalSnapReaderCallbacks *globalSnapReaderCallbacks, OTF2_GlobalSnapReaderCallback_Unknown unknownCallback)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_IdMap.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__IdMap_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <member kind="typedef">
      <type>struct OTF2_IdMap_struct</type>
      <name>OTF2_IdMap</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>a7ed1114a5346fd32eb27ad8435f81088</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>OTF2_IdMap_TraverseCallback</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>af6babbe0298d8b95e4a5038cc01841ea</anchor>
      <arglist>)(uint64_t localId, uint64_t globalId, void *userData)</arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_IdMapMode</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>ac2d193ce3719a97fcf934ea55b4f5ec5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_IdMapMode_enum</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>a87b0013dc8752557c0da97a0ae2b4169</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ID_MAP_DENSE</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>a87b0013dc8752557c0da97a0ae2b4169a16bf9e5f7a884e2e2effdf122c0a8a89</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_ID_MAP_SPARSE</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>a87b0013dc8752557c0da97a0ae2b4169aac93e64f9494c643720932dde555cacc</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_IdMap_AddIdPair</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>a66c3be86de731cf0a2fd6e85ee75ea15</anchor>
      <arglist>(OTF2_IdMap *instance, uint64_t localId, uint64_t globalId)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_IdMap_Clear</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>a67ef1268547721b82c465452502fdaa7</anchor>
      <arglist>(OTF2_IdMap *instance)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_IdMap *</type>
      <name>OTF2_IdMap_Create</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>a815b9f8f981efaa4d41cff606ac69392</anchor>
      <arglist>(OTF2_IdMapMode mode, uint64_t capacity)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_IdMap *</type>
      <name>OTF2_IdMap_CreateFromUint32Array</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>ad018494945dfae56f3283cc8d35815aa</anchor>
      <arglist>(uint64_t length, const uint32_t *mappings, bool optimizeSize)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_IdMap *</type>
      <name>OTF2_IdMap_CreateFromUint64Array</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>ac77de46d9f61ca5044c692e27e42adec</anchor>
      <arglist>(uint64_t length, const uint64_t *mappings, bool optimizeSize)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_IdMap_Free</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>a44c31a09e25835c99969ebe3c2bb3da1</anchor>
      <arglist>(OTF2_IdMap *instance)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_IdMap_GetGlobalId</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>abd56361ee0b740f768939f92d8c78c76</anchor>
      <arglist>(const OTF2_IdMap *instance, uint64_t localId, uint64_t *globalId)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_IdMap_GetMode</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>a0e775fe49338dc760c87721d7de461c7</anchor>
      <arglist>(const OTF2_IdMap *instance, OTF2_IdMapMode *mode)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_IdMap_GetSize</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>ab4fbb9c8fc78718ca369ab65655915db</anchor>
      <arglist>(const OTF2_IdMap *instance, uint64_t *size)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_IdMap_Traverse</name>
      <anchorfile>OTF2__IdMap_8h.html</anchorfile>
      <anchor>a0872de124dae70e634cbd89e5deb6a3a</anchor>
      <arglist>(const OTF2_IdMap *instance, OTF2_IdMap_TraverseCallback callback, void *userData)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_Marker.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__Marker_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__Definitions_8h" name="OTF2_Definitions.h" local="no" imported="no">otf2/OTF2_Definitions.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>OTF2_UNDEFINED_MARKER</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a2c3f11442377be4298ddfaf5bc1f7561</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>OTF2_MarkerRef</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a38bb8fb4bc0a035250c57dd2db15cd08</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_MarkerScope</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a0005ce66acc2f41ca1dcf4e22a82546c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>OTF2_MarkerSeverity</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a91b969addbfc16eeb70e894f8f58ed30</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_MarkerScope_enum</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a1ef5a16fd23f304be8e122d2e4b909f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MARKER_SCOPE_GLOBAL</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a1ef5a16fd23f304be8e122d2e4b909f3a8785ac5954a12c02a1feb564623c9f06</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MARKER_SCOPE_LOCATION</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a1ef5a16fd23f304be8e122d2e4b909f3a9e1a20091412043f71b725f1e5f883b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MARKER_SCOPE_LOCATION_GROUP</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a1ef5a16fd23f304be8e122d2e4b909f3a6efcde77c421e47c7b06760d703a218b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MARKER_SCOPE_SYSTEM_TREE_NODE</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a1ef5a16fd23f304be8e122d2e4b909f3a72d5a21948b4522976035380f4b555b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MARKER_SCOPE_GROUP</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a1ef5a16fd23f304be8e122d2e4b909f3a8e399bba2f19c00a3b8ba08874ea1380</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_MARKER_SCOPE_COMM</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a1ef5a16fd23f304be8e122d2e4b909f3af958e7a70a9b5a52cd4119810643758c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OTF2_MarkerSeverity_enum</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a03b9c17dfb96ec1bda367be24503ce40</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SEVERITY_NONE</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a03b9c17dfb96ec1bda367be24503ce40ad787cc0e4ffa2f12dd18b7f41fc45a89</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SEVERITY_LOW</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a03b9c17dfb96ec1bda367be24503ce40a1e25f9c1f358e5f4b13de958e5f8a8ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SEVERITY_MEDIUM</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a03b9c17dfb96ec1bda367be24503ce40ae6d9fd4f281a23bc51bbdf1aca1b1023</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OTF2_SEVERITY_HIGH</name>
      <anchorfile>OTF2__Marker_8h.html</anchorfile>
      <anchor>a03b9c17dfb96ec1bda367be24503ce40adf037c90a475e22a9243bb6618da2e78</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_MarkerReader.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__MarkerReader_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__Marker_8h" name="OTF2_Marker.h" local="no" imported="no">otf2/OTF2_Marker.h</includes>
    <includes id="OTF2__MarkerReaderCallbacks_8h" name="OTF2_MarkerReaderCallbacks.h" local="no" imported="no">otf2/OTF2_MarkerReaderCallbacks.h</includes>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_MarkerReader_ReadMarkers</name>
      <anchorfile>OTF2__MarkerReader_8h.html</anchorfile>
      <anchor>a96482f1791eeb3e2e0ba6c95d0a09893</anchor>
      <arglist>(OTF2_MarkerReader *reader, uint64_t recordsToRead, uint64_t *recordsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_MarkerReader_SetCallbacks</name>
      <anchorfile>OTF2__MarkerReader_8h.html</anchorfile>
      <anchor>ad860e8b83b9b54974734cf412ae7bf0a</anchor>
      <arglist>(OTF2_MarkerReader *reader, const OTF2_MarkerReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_MarkerReaderCallbacks.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__MarkerReaderCallbacks_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__GeneralDefinitions_8h" name="OTF2_GeneralDefinitions.h" local="no" imported="no">otf2/OTF2_GeneralDefinitions.h</includes>
    <includes id="OTF2__Definitions_8h" name="OTF2_Definitions.h" local="no" imported="no">otf2/OTF2_Definitions.h</includes>
    <includes id="OTF2__IdMap_8h" name="OTF2_IdMap.h" local="no" imported="no">otf2/OTF2_IdMap.h</includes>
    <includes id="OTF2__Marker_8h" name="OTF2_Marker.h" local="no" imported="no">otf2/OTF2_Marker.h</includes>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_MarkerReaderCallback_DefMarker</name>
      <anchorfile>OTF2__MarkerReaderCallbacks_8h.html</anchorfile>
      <anchor>aed2005dc773d123ff0da1ea4ed0e8f5c</anchor>
      <arglist>)(void *userData, OTF2_MarkerRef self, const char *markerGroup, const char *markerCategory, OTF2_MarkerSeverity severity)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_MarkerReaderCallback_Marker</name>
      <anchorfile>OTF2__MarkerReaderCallbacks_8h.html</anchorfile>
      <anchor>abc4ee410a1689de993b2757d0f556503</anchor>
      <arglist>)(void *userData, OTF2_TimeStamp timestamp, OTF2_TimeStamp duration, OTF2_MarkerRef marker, OTF2_MarkerScope scope, uint64_t scopeRef, const char *text)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_MarkerReaderCallback_Unknown</name>
      <anchorfile>OTF2__MarkerReaderCallbacks_8h.html</anchorfile>
      <anchor>a2414a3ea847821c85cd553f0ef31118d</anchor>
      <arglist>)(void *userData)</arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_MarkerReaderCallbacks_struct</type>
      <name>OTF2_MarkerReaderCallbacks</name>
      <anchorfile>OTF2__MarkerReaderCallbacks_8h.html</anchorfile>
      <anchor>a187ff67431d561bbe6406299929c8367</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_MarkerReaderCallbacks_Clear</name>
      <anchorfile>OTF2__MarkerReaderCallbacks_8h.html</anchorfile>
      <anchor>ad9d2ba64948bf03f0c147383e786791b</anchor>
      <arglist>(OTF2_MarkerReaderCallbacks *markerReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_MarkerReaderCallbacks_Delete</name>
      <anchorfile>OTF2__MarkerReaderCallbacks_8h.html</anchorfile>
      <anchor>af92b54b9d5582155077de7730a64a719</anchor>
      <arglist>(OTF2_MarkerReaderCallbacks *markerReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_MarkerReaderCallbacks *</type>
      <name>OTF2_MarkerReaderCallbacks_New</name>
      <anchorfile>OTF2__MarkerReaderCallbacks_8h.html</anchorfile>
      <anchor>ab0a89d4a2f6bfb2db9c15e197754a078</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_MarkerReaderCallbacks_SetDefMarkerCallback</name>
      <anchorfile>OTF2__MarkerReaderCallbacks_8h.html</anchorfile>
      <anchor>a797b19213a8742c7b958c65633b4d6a1</anchor>
      <arglist>(OTF2_MarkerReaderCallbacks *markerReaderCallbacks, OTF2_MarkerReaderCallback_DefMarker defMarkerCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_MarkerReaderCallbacks_SetMarkerCallback</name>
      <anchorfile>OTF2__MarkerReaderCallbacks_8h.html</anchorfile>
      <anchor>a8a78fd61008d5be2054ca7b6275ff3b5</anchor>
      <arglist>(OTF2_MarkerReaderCallbacks *markerReaderCallbacks, OTF2_MarkerReaderCallback_Marker markerCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_MarkerReaderCallbacks_SetUnknownCallback</name>
      <anchorfile>OTF2__MarkerReaderCallbacks_8h.html</anchorfile>
      <anchor>aaaf573e4397201bcba0b0a153fd0d1d6</anchor>
      <arglist>(OTF2_MarkerReaderCallbacks *markerReaderCallbacks, OTF2_MarkerReaderCallback_Unknown unknownCallback)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_MarkerWriter.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__MarkerWriter_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__Definitions_8h" name="OTF2_Definitions.h" local="no" imported="no">otf2/OTF2_Definitions.h</includes>
    <includes id="OTF2__Marker_8h" name="OTF2_Marker.h" local="no" imported="no">otf2/OTF2_Marker.h</includes>
    <member kind="typedef">
      <type>struct OTF2_MarkerWriter_struct</type>
      <name>OTF2_MarkerWriter</name>
      <anchorfile>OTF2__MarkerWriter_8h.html</anchorfile>
      <anchor>afd0b7c32411409f3c88233e254694925</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_MarkerWriter_WriteDefMarker</name>
      <anchorfile>OTF2__MarkerWriter_8h.html</anchorfile>
      <anchor>a4a1b736f0d3d6f452977bbea28e6a679</anchor>
      <arglist>(OTF2_MarkerWriter *writerHandle, OTF2_MarkerRef self, const char *markerGroup, const char *markerCategory, OTF2_MarkerSeverity severity)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_MarkerWriter_WriteMarker</name>
      <anchorfile>OTF2__MarkerWriter_8h.html</anchorfile>
      <anchor>a167908aec99716121d3f63d886ad2ac2</anchor>
      <arglist>(OTF2_MarkerWriter *writerHandle, uint64_t time, uint64_t duration, OTF2_MarkerRef marker, OTF2_MarkerScope scope, uint64_t scopeRef, const char *text)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_Reader.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__Reader_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__Archive_8h" name="OTF2_Archive.h" local="no" imported="no">otf2/OTF2_Archive.h</includes>
    <member kind="typedef">
      <type>struct OTF2_Reader_struct</type>
      <name>OTF2_Reader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a80d8bd8c0a1d91304239f8ce9e77f4b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_Close</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a682fd3726e2687a5dc1849c70961a0f0</anchor>
      <arglist>(OTF2_Reader *reader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_CloseDefReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a7b980ddbeb5781c01df53988011a0c6a</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_DefReader *defReader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_CloseEvtReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a8b76373304e655dac910943c5589ed2f</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_EvtReader *evtReader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_CloseGlobalDefReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>af2563e8b8a0ccd1a7b188ae0924711b6</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalDefReader *globalDefReader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_CloseGlobalEvtReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a80b4eeb1b031d3619852ffd744c0db38</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalEvtReader *globalEvtReader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_CloseGlobalSnapReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a1226334c535b0bbd68da5a4be8aecbc2</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalSnapReader *globalSnapReader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_CloseMarkerReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a59d95319f05cc30b8c03f42ba3f4ade7</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_MarkerReader *markerReader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_CloseMarkerWriter</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>af40f58fb50aea1f2d3536f8079aae523</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_MarkerWriter *markerWriter)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_CloseSnapReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a13f4e94e792e7ae8918f4bcacf001bc5</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_SnapReader *snapReader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_CloseThumbReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>aef9f23ce6231445e33043c8e8437befd</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_ThumbReader *thumbReader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetBoolProperty</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a0aff681c734dc4f6bb8149c008d72549</anchor>
      <arglist>(OTF2_Reader *reader, const char *name, bool *value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetChunkSize</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a410613dac71480d2e58932b82454b0bf</anchor>
      <arglist>(OTF2_Reader *reader, uint64_t *chunkSizeEvents, uint64_t *chunkSizeDefinitions)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetCompression</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a1627b560304f1dde59b5fe9fca6d9962</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_Compression *compression)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetCreator</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>afc605fe424922f08776797410f3b8dde</anchor>
      <arglist>(OTF2_Reader *reader, char **creator)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_DefReader *</type>
      <name>OTF2_Reader_GetDefReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a6492f861791446897de466a48913cb2b</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_LocationRef location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetDescription</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a6540ba203d9d50e0377b8d96592c9bc8</anchor>
      <arglist>(OTF2_Reader *reader, char **description)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_EvtReader *</type>
      <name>OTF2_Reader_GetEvtReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>ad3b5e2711f994d4253d3bf901c42e3a8</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_LocationRef location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetFileSubstrate</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a8e9df35221f3a8f19a8ca94ab512b5a0</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_FileSubstrate *substrate)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_GlobalDefReader *</type>
      <name>OTF2_Reader_GetGlobalDefReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a04ac6864d8fac31bc162b91028412fed</anchor>
      <arglist>(OTF2_Reader *reader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_GlobalEvtReader *</type>
      <name>OTF2_Reader_GetGlobalEvtReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>ac5bad892325d6de894c937e13c08cb06</anchor>
      <arglist>(OTF2_Reader *reader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_GlobalSnapReader *</type>
      <name>OTF2_Reader_GetGlobalSnapReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a3653431917c3684a42d3ad4edddea19b</anchor>
      <arglist>(OTF2_Reader *reader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetMachineName</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a100e99310438fd7473853ff76da0a907</anchor>
      <arglist>(OTF2_Reader *reader, char **machineName)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_MarkerReader *</type>
      <name>OTF2_Reader_GetMarkerReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a5d52127e2d428d7c99ccbf58b240e2e5</anchor>
      <arglist>(OTF2_Reader *reader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_MarkerWriter *</type>
      <name>OTF2_Reader_GetMarkerWriter</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a71159c61fad7b27bad3ae87c1ee1cb2b</anchor>
      <arglist>(OTF2_Reader *reader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetNumberOfGlobalDefinitions</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a8250303e65f4d33aa98c5b56f752c93d</anchor>
      <arglist>(OTF2_Reader *reader, uint64_t *numberOfDefinitions)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetNumberOfLocations</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a19628e6ff9ba7531f5d4e56d3744c69b</anchor>
      <arglist>(OTF2_Reader *reader, uint64_t *numberOfLocations)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetNumberOfSnapshots</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a9f20e4a56cfef5659b9ba660bd4fbfa8</anchor>
      <arglist>(OTF2_Reader *reader, uint32_t *number)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetNumberOfThumbnails</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a1253bdfd80930fe9ab6ff8d9b3b6e654</anchor>
      <arglist>(OTF2_Reader *reader, uint32_t *number)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetProperty</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>af2b1393e61ecc3b66e6f993273921794</anchor>
      <arglist>(OTF2_Reader *reader, const char *name, char **value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetPropertyNames</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a3ff1c3f00bb5da64d041b3b4776157f8</anchor>
      <arglist>(OTF2_Reader *reader, uint32_t *numberOfProperties, char ***names)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_SnapReader *</type>
      <name>OTF2_Reader_GetSnapReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>aa705cae539ed9764c74f48f8b36ca0e6</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_LocationRef location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ThumbReader *</type>
      <name>OTF2_Reader_GetThumbReader</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a929f4ff9b66d61d444406fd9d1c77fb9</anchor>
      <arglist>(OTF2_Reader *reader, uint32_t number)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetTraceId</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>af2d53cc141c1d8fd39d1e1d49dce23a8</anchor>
      <arglist>(OTF2_Reader *reader, uint64_t *id)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_GetVersion</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a5f0c8c102ac988a2d33cd2c5a8a07a4b</anchor>
      <arglist>(OTF2_Reader *reader, uint8_t *major, uint8_t *minor, uint8_t *bugfix)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_HasGlobalEvent</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>aeb07b80002c07ea1d0c11a197963786e</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalEvtReader *evtReader, int *flag)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_Reader *</type>
      <name>OTF2_Reader_Open</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a1ceb972f5fd94e756240291c0982bac4</anchor>
      <arglist>(const char *anchorFilePath)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadAllGlobalDefinitions</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a675f7dbf1b8783de10f52396769bc957</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalDefReader *defReader, uint64_t *definitionsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadAllGlobalEvents</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a141742794ae94c5c827dc7fd8a6ca5f3</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalEvtReader *evtReader, uint64_t *eventsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadAllGlobalSnapshots</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a6efec631db30dab374edda04849f333e</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalSnapReader *snapReader, uint64_t *recordsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadAllLocalDefinitions</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>aeb2dac6ba415abbed5c24bcbf498d4f8</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_DefReader *defReader, uint64_t *definitionsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadAllLocalEvents</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a7cd176155d3925d0ebb61ec6ee1f099a</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_EvtReader *evtReader, uint64_t *eventsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadAllLocalSnapshots</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>abb9ecc09e05e047b1b479adc0066bb82</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_SnapReader *snapReader, uint64_t *recordsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadAllMarkers</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>ab484e8507c0d042a41356aff222ceb32</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_MarkerReader *markerReader, uint64_t *markersRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadGlobalDefinitions</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>afe89c10e757413c700a3e6d39b045c97</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalDefReader *defReader, uint64_t definitionsToRead, uint64_t *definitionsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadGlobalEvent</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a39981aac0d766ddf5a48d1ae838f98c6</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalEvtReader *evtReader)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadGlobalEvents</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a234a1d5d6d5f6537937979b3796013a5</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalEvtReader *evtReader, uint64_t eventsToRead, uint64_t *eventsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadGlobalSnapshots</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a1de7c2fffe9669cddce0c5adc637c786</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalSnapReader *snapReader, uint64_t recordsToRead, uint64_t *recordsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadLocalDefinitions</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a41734760db149d548e46d982eae2daef</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_DefReader *defReader, uint64_t definitionsToRead, uint64_t *definitionsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadLocalEvents</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>aa7c18d9e79a036ef203b5e881d97eec5</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_EvtReader *evtReader, uint64_t eventsToRead, uint64_t *eventsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadLocalEventsBackward</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a5344c9cf42572c1b7d34c6145297eb75</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_EvtReader *evtReader, uint64_t eventsToRead, uint64_t *eventsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadLocalSnapshots</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a5c75221268dd44aadc42cbc31752f558</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_SnapReader *snapReader, uint64_t recordsToRead, uint64_t *recordsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_ReadMarkers</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>ae2133b98c2dc2f194e211825d651f060</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_MarkerReader *markerReader, uint64_t markersToRead, uint64_t *markersRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_RegisterDefCallbacks</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a0b3ca31ed86e44ddee74b3161b099ac8</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_DefReader *defReader, const OTF2_DefReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_RegisterEvtCallbacks</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a55caa00d3b965f8944661818eb238a43</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_EvtReader *evtReader, const OTF2_EvtReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_RegisterGlobalDefCallbacks</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a74bf926ec84e576cc59602db57a6865c</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalDefReader *defReader, const OTF2_GlobalDefReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_RegisterGlobalEvtCallbacks</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>ac0ef459392c6bd1b35f5ad1edfc5c927</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalEvtReader *evtReader, const OTF2_GlobalEvtReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_RegisterGlobalSnapCallbacks</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>a785c11588373e0c139022118a8a00e94</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_GlobalSnapReader *evtReader, const OTF2_GlobalSnapReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_RegisterMarkerCallbacks</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>ab1fc6ff708cb4511b4ddc4f422e7e936</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_MarkerReader *markerReader, const OTF2_MarkerReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_RegisterSnapCallbacks</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>ab4f9e99f5c0664718194ea60b3d14822</anchor>
      <arglist>(OTF2_Reader *reader, OTF2_SnapReader *snapReader, const OTF2_SnapReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_Reader_SetFileSionCallbacks</name>
      <anchorfile>OTF2__Reader_8h.html</anchorfile>
      <anchor>accb5437eef350cde8bd245c4256f3c18</anchor>
      <arglist>(OTF2_Reader *reader, const OTF2_FileSionCallbacks *fileSionCallbacks, void *fileSionData)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_SnapReader.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__SnapReader_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__Events_8h" name="OTF2_Events.h" local="no" imported="no">otf2/OTF2_Events.h</includes>
    <includes id="OTF2__Definitions_8h" name="OTF2_Definitions.h" local="no" imported="no">otf2/OTF2_Definitions.h</includes>
    <includes id="OTF2__AttributeList_8h" name="OTF2_AttributeList.h" local="no" imported="no">otf2/OTF2_AttributeList.h</includes>
    <includes id="OTF2__SnapReaderCallbacks_8h" name="OTF2_SnapReaderCallbacks.h" local="no" imported="no">otf2/OTF2_SnapReaderCallbacks.h</includes>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReader_GetLocationID</name>
      <anchorfile>OTF2__SnapReader_8h.html</anchorfile>
      <anchor>aca77889a6a27a046ffad3039d5cd6248</anchor>
      <arglist>(const OTF2_SnapReader *reader, OTF2_LocationRef *location)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReader_ReadSnapshots</name>
      <anchorfile>OTF2__SnapReader_8h.html</anchorfile>
      <anchor>af81726056553501d443bd80bcfe90657</anchor>
      <arglist>(OTF2_SnapReader *reader, uint64_t recordsToRead, uint64_t *recordsRead)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReader_Seek</name>
      <anchorfile>OTF2__SnapReader_8h.html</anchorfile>
      <anchor>a7dbde99a5ffd5b08cef32d5bddfa79fe</anchor>
      <arglist>(OTF2_SnapReader *reader, uint64_t req_time, bool *found)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReader_SetCallbacks</name>
      <anchorfile>OTF2__SnapReader_8h.html</anchorfile>
      <anchor>a92743bbb11b292abd0d6dc14e7fa8eff</anchor>
      <arglist>(OTF2_SnapReader *reader, const OTF2_SnapReaderCallbacks *callbacks, void *userData)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_SnapReaderCallbacks.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__SnapReaderCallbacks_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__GeneralDefinitions_8h" name="OTF2_GeneralDefinitions.h" local="no" imported="no">otf2/OTF2_GeneralDefinitions.h</includes>
    <includes id="OTF2__AttributeList_8h" name="OTF2_AttributeList.h" local="no" imported="no">otf2/OTF2_AttributeList.h</includes>
    <includes id="OTF2__Events_8h" name="OTF2_Events.h" local="no" imported="no">otf2/OTF2_Events.h</includes>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_Enter</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a12b820f4eb4f37c316fa88130bba4295</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_RegionRef region)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_MeasurementOnOff</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a5122e260e3b72e1907486436fdb78172</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_MeasurementMode measurementMode)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_Metric</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>adc44cce8ece1ff5b1e9a8352879d8eb9</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_MetricRef metric, uint8_t numberOfMetrics, const OTF2_Type *typeIDs, const OTF2_MetricValue *metricValues)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_MpiCollectiveBegin</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>adc8cc48c369c66d21cdc8da1a3f12980</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_MpiCollectiveEnd</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a55c6b5026a93e9f04982ca2e0628574d</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_CollectiveOp collectiveOp, OTF2_CommRef communicator, uint32_t root, uint64_t sizeSent, uint64_t sizeReceived)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_MpiIrecv</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a25845c940938f36027c199b32b515b1c</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint32_t sender, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_MpiIrecvRequest</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>ae00fd716f2ee70852fad3b2aea8046ed</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_MpiIsend</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a3ef0b0eaa6eb408d6e92416d5e5485ef</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint32_t receiver, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_MpiIsendComplete</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a2a90b1ace40f8141fe0fee63102c9f8d</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint64_t requestID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_MpiRecv</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>af9269fd1b0a6eaa87c05b3c46c434895</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint32_t sender, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_MpiSend</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a50fd65b49ec9bbf3d255386b1e359ce2</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint32_t receiver, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_OmpAcquireLock</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a9bf65c1829c0c4cf20ff4a7beb580fb8</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_OmpFork</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a4982d9ef31e957a9440492bc7c2a8e80</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint32_t numberOfRequestedThreads)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_OmpTaskCreate</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a727771d3e6456567afa16695a577ebe5</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint64_t taskID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_OmpTaskSwitch</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>ac378b0d968af226413908dbf7f063a64</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, uint64_t taskID)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_ParameterInt</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a5b723bb6dec2520838f3d9d523ba2f24</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_ParameterRef parameter, int64_t value)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_ParameterString</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a54336446b83472f5e96120c3f3240f38</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_ParameterRef parameter, OTF2_StringRef string)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_ParameterUnsignedInt</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a19ff114a4dff98726db82e2c0342fe4b</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, OTF2_TimeStamp origEventTime, OTF2_ParameterRef parameter, uint64_t value)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_SnapshotEnd</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a33746e582d1492506e1b366b601a87f2</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, uint64_t contReadPos)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_SnapshotStart</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>af49d3f65fa1511c3032b2d68954e715c</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList, uint64_t numberOfRecords)</arglist>
    </member>
    <member kind="typedef">
      <type>OTF2_CallbackCode(*</type>
      <name>OTF2_SnapReaderCallback_Unknown</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>ac71a8e62e2c5fa7c7b190c24a5e13d8c</anchor>
      <arglist>)(OTF2_LocationRef location, OTF2_TimeStamp snapTime, void *userData, OTF2_AttributeList *attributeList)</arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_SnapReaderCallbacks_struct</type>
      <name>OTF2_SnapReaderCallbacks</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>aa742e3490e9c40ddbd893731616f430d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_SnapReaderCallbacks_Clear</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a720f1b76f116e08ee48491d0561bba40</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OTF2_SnapReaderCallbacks_Delete</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>ae55faecc1282f53b8b6c1e763c84ea5e</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_SnapReaderCallbacks *</type>
      <name>OTF2_SnapReaderCallbacks_New</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>abe9444578e0e60c6e996ab485bcdbf01</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetEnterCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a8281c97e14647de77cf2902a19a7aab9</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_Enter enterCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetMeasurementOnOffCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>adbc7d4ee2d35cab417b0c641a1457fbe</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_MeasurementOnOff measurementOnOffCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetMetricCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>ae4a4ee4aef59804e28da3ca850805af6</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_Metric metricCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetMpiCollectiveBeginCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a5c432b8d0b80e73b4d8f9108483c91e4</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_MpiCollectiveBegin mpiCollectiveBeginCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetMpiCollectiveEndCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>ab8d5dbd0570c55c4d399e8c80db4654b</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_MpiCollectiveEnd mpiCollectiveEndCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetMpiIrecvCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>acb059a4e3898e7685c7baaac17a1f1d6</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_MpiIrecv mpiIrecvCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetMpiIrecvRequestCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a079fede80d9f17d0811dc3ab167117a8</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_MpiIrecvRequest mpiIrecvRequestCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetMpiIsendCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a649bc03cc56b1f09fbaf57345be51a40</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_MpiIsend mpiIsendCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetMpiIsendCompleteCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a47362c9507f8d3d1851355f5f4e4df0e</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_MpiIsendComplete mpiIsendCompleteCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetMpiRecvCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a9a8b239a5dce577721652fe690262b09</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_MpiRecv mpiRecvCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetMpiSendCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a89e19d46a57f964524bd540a1e4d34c9</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_MpiSend mpiSendCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetOmpAcquireLockCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a080aac4cfb7cde9ee831969c12350470</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_OmpAcquireLock ompAcquireLockCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetOmpForkCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a1b20798316876dcb71ec4c0ff2096435</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_OmpFork ompForkCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetOmpTaskCreateCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a631e3c959f658e2db9ade262127a4b0d</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_OmpTaskCreate ompTaskCreateCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetOmpTaskSwitchCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>af72d41e91822ac29487cc22f7596a764</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_OmpTaskSwitch ompTaskSwitchCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetParameterIntCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>aa1ea08613b3d5e0de5cb782e96b5ade8</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_ParameterInt parameterIntCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetParameterStringCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a8c79d2d62adced1aba8e29411f505810</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_ParameterString parameterStringCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetParameterUnsignedIntCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a62b9f00c075d563a293f1855009c51bf</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_ParameterUnsignedInt parameterUnsignedIntCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetSnapshotEndCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a8046079ff593cc2452aca0b8706d4b12</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_SnapshotEnd snapshotEndCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetSnapshotStartCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>a159e3a4e466138245c76e4ccd0cf62cb</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_SnapshotStart snapshotStartCallback)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapReaderCallbacks_SetUnknownCallback</name>
      <anchorfile>OTF2__SnapReaderCallbacks_8h.html</anchorfile>
      <anchor>aa2683b410d89110268ad50afee69bc3c</anchor>
      <arglist>(OTF2_SnapReaderCallbacks *snapReaderCallbacks, OTF2_SnapReaderCallback_Unknown unknownCallback)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_SnapWriter.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__SnapWriter_8h</filename>
    <includes id="OTF2__ErrorCodes_8h" name="OTF2_ErrorCodes.h" local="no" imported="no">otf2/OTF2_ErrorCodes.h</includes>
    <includes id="OTF2__Events_8h" name="OTF2_Events.h" local="no" imported="no">otf2/OTF2_Events.h</includes>
    <includes id="OTF2__AttributeList_8h" name="OTF2_AttributeList.h" local="no" imported="no">otf2/OTF2_AttributeList.h</includes>
    <member kind="typedef">
      <type>struct OTF2_SnapWriter_struct</type>
      <name>OTF2_SnapWriter</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>ae2d3e52d61cb12f8a91ea983de56b2fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_Enter</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>a21eeb174aced456d5702b903a973c6ec</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, OTF2_RegionRef region)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_GetLocationID</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>ad0bd39eb178ca64b04d798fb2207100e</anchor>
      <arglist>(const OTF2_SnapWriter *writer, OTF2_LocationRef *locationID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_MeasurementOnOff</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>a53121eb754cbe9c833ad67735d999654</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, OTF2_MeasurementMode measurementMode)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_Metric</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>a384fe019c3ea9c74abe36a16a977e1a4</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, OTF2_MetricRef metric, uint8_t numberOfMetrics, const OTF2_Type *typeIDs, const OTF2_MetricValue *metricValues)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_MpiCollectiveBegin</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>ad4877f6c0c18d790ba61dea1309e3d2b</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_MpiCollectiveEnd</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>af1d752df2fe1973c8d1b96828a46cea1</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, OTF2_CollectiveOp collectiveOp, OTF2_CommRef communicator, uint32_t root, uint64_t sizeSent, uint64_t sizeReceived)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_MpiIrecv</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>a36362952c5b9a881d6d98b3baa41185e</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, uint32_t sender, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength, uint64_t requestID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_MpiIrecvRequest</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>aac83ea2294294375a5cd6ca08d33052b</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, uint64_t requestID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_MpiIsend</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>ad59fa0ed5c67a8016fcb26431a11a9ca</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, uint32_t receiver, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength, uint64_t requestID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_MpiIsendComplete</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>afbe119614910740ecbc58e470424b320</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, uint64_t requestID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_MpiRecv</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>a4570ab5aad85b95d9ea577ae11ff25a2</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, uint32_t sender, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_MpiSend</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>ae8b650471035224b707f671c2f03ad51</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, uint32_t receiver, OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_OmpAcquireLock</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>a1e23db52ed41fa2f4bfcafc24bcf8654</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, uint32_t lockID, uint32_t acquisitionOrder)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_OmpFork</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>ae26cdcf424c99f94c77144842af95d7e</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, uint32_t numberOfRequestedThreads)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_OmpTaskCreate</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>a730a1187debbab73bde59bc24a71fa77</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, uint64_t taskID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_OmpTaskSwitch</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>a9f4803ba837f67cf24d55f7fcdc1acce</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, uint64_t taskID)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_ParameterInt</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>a0a85766362db9f2275a652ad80d56784</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, OTF2_ParameterRef parameter, int64_t value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_ParameterString</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>a8892f7d3abc43d1884428e555e31abf4</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, OTF2_ParameterRef parameter, OTF2_StringRef string)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_ParameterUnsignedInt</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>a981845b77492ec7310b7a68c9bf88b13</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, OTF2_TimeStamp origEventTime, OTF2_ParameterRef parameter, uint64_t value)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_SnapshotEnd</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>a8badf899cf7c3bde1c629b2600791e6d</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, uint64_t contReadPos)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_SnapWriter_SnapshotStart</name>
      <anchorfile>OTF2__SnapWriter_8h.html</anchorfile>
      <anchor>af6236102b7d96edd8fe0ee18a116c8fa</anchor>
      <arglist>(OTF2_SnapWriter *writer, OTF2_AttributeList *attributeList, OTF2_TimeStamp snapTime, uint64_t numberOfRecords)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OTF2_Thumbnail.h</name>
    <path>/home/roessel/dev/otf2/src/tags/REL-1.2.1/include/otf2/</path>
    <filename>OTF2__Thumbnail_8h</filename>
    <includes id="OTF2__GeneralDefinitions_8h" name="OTF2_GeneralDefinitions.h" local="no" imported="no">otf2/OTF2_GeneralDefinitions.h</includes>
    <member kind="typedef">
      <type>struct OTF2_ThumbReader_struct</type>
      <name>OTF2_ThumbReader</name>
      <anchorfile>OTF2__Thumbnail_8h.html</anchorfile>
      <anchor>a32170ea8a3094e954b4bf171303aac59</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct OTF2_ThumbWriter_struct</type>
      <name>OTF2_ThumbWriter</name>
      <anchorfile>OTF2__Thumbnail_8h.html</anchorfile>
      <anchor>a1e1bd1c8f761df20eda0f4f6dc226836</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_ThumbReader_GetHeader</name>
      <anchorfile>OTF2__Thumbnail_8h.html</anchorfile>
      <anchor>a128b0879317d9eccf0247beb6a14052a</anchor>
      <arglist>(OTF2_ThumbReader *reader, char **const name, char **const description, OTF2_ThumbnailType *type, uint32_t *numberOfSamples, uint32_t *numberOfMetrics, uint64_t **refsToDefs)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_ThumbReader_ReadSample</name>
      <anchorfile>OTF2__Thumbnail_8h.html</anchorfile>
      <anchor>a0f91c826c51312c319d24ad4516726fa</anchor>
      <arglist>(OTF2_ThumbReader *reader, uint64_t *baseline, uint32_t numberOfMetrics, uint64_t *metricSamples)</arglist>
    </member>
    <member kind="function">
      <type>OTF2_ErrorCode</type>
      <name>OTF2_ThumbWriter_WriteSample</name>
      <anchorfile>OTF2__Thumbnail_8h.html</anchorfile>
      <anchor>a901a8800e286ec8023a746fb94796e80</anchor>
      <arglist>(OTF2_ThumbWriter *writer, uint64_t baseline, uint32_t numberOfMetrics, const uint64_t *metricSamples)</arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>tools_documentation</name>
    <title>OTF2 Tools</title>
    <filename>tools_documentation</filename>
    <docanchor file="tools_documentation">otf2_print_tool</docanchor>
    <docanchor file="tools_documentation">otf2_marker_tool</docanchor>
    <docanchor file="tools_documentation">otf2_snapshots_tool</docanchor>
    <docanchor file="tools_documentation">usage_tools</docanchor>
    <docanchor file="tools_documentation">otf2_config_tool</docanchor>
  </compound>
  <compound kind="page">
    <name>installationfile</name>
    <title>OTF2 INSTALL</title>
    <filename>installationfile</filename>
  </compound>
  <compound kind="page">
    <name>definition_records</name>
    <title>List of all definition records</title>
    <filename>definition_records</filename>
    <docanchor file="definition_records">SystemTreeNodeDomain_systemTreeNode</docanchor>
    <docanchor file="definition_records">Parameter_parameterType</docanchor>
    <docanchor file="definition_records">Parameter_name</docanchor>
    <docanchor file="definition_records">MetricMember_unit</docanchor>
    <docanchor file="definition_records">MetricMember_valueType</docanchor>
    <docanchor file="definition_records">Region_paradigm</docanchor>
    <docanchor file="definition_records">String</docanchor>
    <docanchor file="definition_records">ClockOffset_time</docanchor>
    <docanchor file="definition_records">MetricClassRecorder_recorder</docanchor>
    <docanchor file="definition_records">LocationGroup_name</docanchor>
    <docanchor file="definition_records">ClockOffset</docanchor>
    <docanchor file="definition_records">SystemTreeNodeProperty_name</docanchor>
    <docanchor file="definition_records">MetricInstance_recorder</docanchor>
    <docanchor file="definition_records">MetricMember</docanchor>
    <docanchor file="definition_records">Callpath</docanchor>
    <docanchor file="definition_records">Location_locationGroup</docanchor>
    <docanchor file="definition_records">SystemTreeNodeProperty</docanchor>
    <docanchor file="definition_records">Callpath_parent</docanchor>
    <docanchor file="definition_records">Callsite_lineNumber</docanchor>
    <docanchor file="definition_records">Region_beginLineNumber</docanchor>
    <docanchor file="definition_records">Region</docanchor>
    <docanchor file="definition_records">LocationGroup_locationGroupType</docanchor>
    <docanchor file="definition_records">ClockProperties</docanchor>
    <docanchor file="definition_records">Parameter</docanchor>
    <docanchor file="definition_records">Comm_name</docanchor>
    <docanchor file="definition_records">Group_groupFlags</docanchor>
    <docanchor file="definition_records">Callsite</docanchor>
    <docanchor file="definition_records">SystemTreeNodeDomain_systemTreeDomain</docanchor>
    <docanchor file="definition_records">RmaWin</docanchor>
    <docanchor file="definition_records">MetricClass_recorderKind</docanchor>
    <docanchor file="definition_records">MetricClass_numberOfMetrics</docanchor>
    <docanchor file="definition_records">Group_numberOfMembers</docanchor>
    <docanchor file="definition_records">Callsite_sourceFile</docanchor>
    <docanchor file="definition_records">Region_endLineNumber</docanchor>
    <docanchor file="definition_records">SystemTreeNode_className</docanchor>
    <docanchor file="definition_records">SystemTreeNodeProperty_systemTreeNode</docanchor>
    <docanchor file="definition_records">MetricClassRecorder_metricClass</docanchor>
    <docanchor file="definition_records">Comm_parent</docanchor>
    <docanchor file="definition_records">LocationGroup_systemTreeParent</docanchor>
    <docanchor file="definition_records">MetricMember_exponent</docanchor>
    <docanchor file="definition_records">Group_paradigm</docanchor>
    <docanchor file="definition_records">Callsite_enteredRegion</docanchor>
    <docanchor file="definition_records">Attribute_type</docanchor>
    <docanchor file="definition_records">MappingTable</docanchor>
    <docanchor file="definition_records">SystemTreeNodeProperty_value</docanchor>
    <docanchor file="definition_records">MetricInstance</docanchor>
    <docanchor file="definition_records">MetricClass_metricMembers</docanchor>
    <docanchor file="definition_records">Region_sourceFile</docanchor>
    <docanchor file="definition_records">MetricMember_description</docanchor>
    <docanchor file="definition_records">Region_regionFlags</docanchor>
    <docanchor file="definition_records">MappingTable_mappingType</docanchor>
    <docanchor file="definition_records">RmaWin_name</docanchor>
    <docanchor file="definition_records">Comm</docanchor>
    <docanchor file="definition_records">MetricInstance_scope</docanchor>
    <docanchor file="definition_records">MetricMember_metricType</docanchor>
    <docanchor file="definition_records">MetricMember_name</docanchor>
    <docanchor file="definition_records">Group_groupType</docanchor>
    <docanchor file="definition_records">Region_name</docanchor>
    <docanchor file="definition_records">String_string</docanchor>
    <docanchor file="definition_records">LocationGroup</docanchor>
    <docanchor file="definition_records">Attribute_name</docanchor>
    <docanchor file="definition_records">SystemTreeNodeDomain</docanchor>
    <docanchor file="definition_records">Group</docanchor>
    <docanchor file="definition_records">Region_canonicalName</docanchor>
    <docanchor file="definition_records">Location_name</docanchor>
    <docanchor file="definition_records">ClockProperties_traceLength</docanchor>
    <docanchor file="definition_records">ClockProperties_globalOffset</docanchor>
    <docanchor file="definition_records">MetricClassRecorder</docanchor>
    <docanchor file="definition_records">MetricInstance_metricClass</docanchor>
    <docanchor file="definition_records">Group_members</docanchor>
    <docanchor file="definition_records">Group_name</docanchor>
    <docanchor file="definition_records">Callpath_region</docanchor>
    <docanchor file="definition_records">Location</docanchor>
    <docanchor file="definition_records">SystemTreeNode_parent</docanchor>
    <docanchor file="definition_records">Attribute</docanchor>
    <docanchor file="definition_records">Comm_group</docanchor>
    <docanchor file="definition_records">MetricClass</docanchor>
    <docanchor file="definition_records">MetricMember_metricBase</docanchor>
    <docanchor file="definition_records">SystemTreeNode_name</docanchor>
    <docanchor file="definition_records">ClockOffset_standardDeviation</docanchor>
    <docanchor file="definition_records">RmaWin_comm</docanchor>
    <docanchor file="definition_records">MetricClass_metricOccurrence</docanchor>
    <docanchor file="definition_records">Region_description</docanchor>
    <docanchor file="definition_records">Location_numberOfEvents</docanchor>
    <docanchor file="definition_records">MappingTable_idMap</docanchor>
    <docanchor file="definition_records">ClockProperties_timerResolution</docanchor>
    <docanchor file="definition_records">MetricInstance_metricScope</docanchor>
    <docanchor file="definition_records">MetricMember_metricMode</docanchor>
    <docanchor file="definition_records">Callsite_leftRegion</docanchor>
    <docanchor file="definition_records">Region_regionRole</docanchor>
    <docanchor file="definition_records">Location_locationType</docanchor>
    <docanchor file="definition_records">SystemTreeNode</docanchor>
    <docanchor file="definition_records">ClockOffset_offset</docanchor>
  </compound>
  <compound kind="page">
    <name>event_records</name>
    <title>List of all event records</title>
    <filename>event_records</filename>
    <docanchor file="event_records">RmaOpTest_win</docanchor>
    <docanchor file="event_records">RmaGet_bytes</docanchor>
    <docanchor file="event_records">RmaGet_win</docanchor>
    <docanchor file="event_records">RmaSync_syncType</docanchor>
    <docanchor file="event_records">RmaSync_win</docanchor>
    <docanchor file="event_records">RmaAcquireLock_win</docanchor>
    <docanchor file="event_records">RmaRequestLock_remote</docanchor>
    <docanchor file="event_records">MpiIsend_communicator</docanchor>
    <docanchor file="event_records">MpiSend_communicator</docanchor>
    <docanchor file="event_records">ThreadTaskCreate_threadTeam</docanchor>
    <docanchor file="event_records">ThreadTeamBegin_threadTeam</docanchor>
    <docanchor file="event_records">RmaOpCompleteRemote_matchingId</docanchor>
    <docanchor file="event_records">RmaOpCompleteRemote</docanchor>
    <docanchor file="event_records">RmaTryLock</docanchor>
    <docanchor file="event_records">RmaCollectiveEnd_bytesSent</docanchor>
    <docanchor file="event_records">RmaWinDestroy_win</docanchor>
    <docanchor file="event_records">MpiRequestTest</docanchor>
    <docanchor file="event_records">MpiSend</docanchor>
    <docanchor file="event_records">MeasurementOnOff</docanchor>
    <docanchor file="event_records">ThreadReleaseLock_model</docanchor>
    <docanchor file="event_records">RmaOpCompleteRemote_win</docanchor>
    <docanchor file="event_records">RmaWaitChange_win</docanchor>
    <docanchor file="event_records">RmaReleaseLock</docanchor>
    <docanchor file="event_records">RmaTryLock_win</docanchor>
    <docanchor file="event_records">MpiRecv_msgLength</docanchor>
    <docanchor file="event_records">MpiIrecvRequest_requestID</docanchor>
    <docanchor file="event_records">MpiIsend</docanchor>
    <docanchor file="event_records">RmaAtomic_matchingId</docanchor>
    <docanchor file="event_records">RmaGet_matchingId</docanchor>
    <docanchor file="event_records">RmaSync</docanchor>
    <docanchor file="event_records">RmaAcquireLock_lockId</docanchor>
    <docanchor file="event_records">RmaWinCreate</docanchor>
    <docanchor file="event_records">OmpReleaseLock_acquisitionOrder</docanchor>
    <docanchor file="event_records">MpiIsend_requestID</docanchor>
    <docanchor file="event_records">MpiSend_msgTag</docanchor>
    <docanchor file="event_records">Enter</docanchor>
    <docanchor file="event_records">ThreadAcquireLock_acquisitionOrder</docanchor>
    <docanchor file="event_records">RmaOpCompleteNonBlocking_matchingId</docanchor>
    <docanchor file="event_records">RmaAtomic_win</docanchor>
    <docanchor file="event_records">RmaPut_remote</docanchor>
    <docanchor file="event_records">ParameterString_parameter</docanchor>
    <docanchor file="event_records">Leave</docanchor>
    <docanchor file="event_records">ThreadTaskCreate_generationNumber</docanchor>
    <docanchor file="event_records">ThreadReleaseLock_lockID</docanchor>
    <docanchor file="event_records">RmaAtomic_type</docanchor>
    <docanchor file="event_records">RmaPut_bytes</docanchor>
    <docanchor file="event_records">RmaReleaseLock_win</docanchor>
    <docanchor file="event_records">RmaTryLock_remote</docanchor>
    <docanchor file="event_records">RmaAcquireLock</docanchor>
    <docanchor file="event_records">RmaRequestLock_lockType</docanchor>
    <docanchor file="event_records">Metric_metric</docanchor>
    <docanchor file="event_records">OmpTaskCreate_taskID</docanchor>
    <docanchor file="event_records">OmpReleaseLock_lockID</docanchor>
    <docanchor file="event_records">MpiRecv_msgTag</docanchor>
    <docanchor file="event_records">MpiRecv_communicator</docanchor>
    <docanchor file="event_records">MpiIrecvRequest</docanchor>
    <docanchor file="event_records">RmaAtomic_remote</docanchor>
    <docanchor file="event_records">RmaRequestLock_lockId</docanchor>
    <docanchor file="event_records">RmaGroupSync_win</docanchor>
    <docanchor file="event_records">RmaWinCreate_win</docanchor>
    <docanchor file="event_records">ParameterUnsignedInt_parameter</docanchor>
    <docanchor file="event_records">ParameterString_string</docanchor>
    <docanchor file="event_records">OmpTaskCreate</docanchor>
    <docanchor file="event_records">MpiCollectiveEnd_communicator</docanchor>
    <docanchor file="event_records">MpiRecv_sender</docanchor>
    <docanchor file="event_records">MpiRecv</docanchor>
    <docanchor file="event_records">MpiIsendComplete</docanchor>
    <docanchor file="event_records">ThreadAcquireLock</docanchor>
    <docanchor file="event_records">ThreadJoin</docanchor>
    <docanchor file="event_records">RmaOpTest</docanchor>
    <docanchor file="event_records">RmaOpCompleteBlocking_matchingId</docanchor>
    <docanchor file="event_records">OmpFork_numberOfRequestedThreads</docanchor>
    <docanchor file="event_records">MpiIrecv</docanchor>
    <docanchor file="event_records">ThreadTaskComplete_generationNumber</docanchor>
    <docanchor file="event_records">ThreadTeamEnd_threadTeam</docanchor>
    <docanchor file="event_records">RmaAtomic</docanchor>
    <docanchor file="event_records">RmaPut_matchingId</docanchor>
    <docanchor file="event_records">OmpAcquireLock_acquisitionOrder</docanchor>
    <docanchor file="event_records">MpiCollectiveEnd_sizeSent</docanchor>
    <docanchor file="event_records">BufferFlush_stopTime</docanchor>
    <docanchor file="event_records">ThreadAcquireLock_model</docanchor>
    <docanchor file="event_records">RmaPut</docanchor>
    <docanchor file="event_records">RmaSync_remote</docanchor>
    <docanchor file="event_records">ParameterInt_value</docanchor>
    <docanchor file="event_records">MpiCollectiveEnd_sizeReceived</docanchor>
    <docanchor file="event_records">MpiCollectiveEnd</docanchor>
    <docanchor file="event_records">MpiIrecv_msgTag</docanchor>
    <docanchor file="event_records">MpiIsend_receiver</docanchor>
    <docanchor file="event_records">Leave_region</docanchor>
    <docanchor file="event_records">ThreadTaskSwitch</docanchor>
    <docanchor file="event_records">ThreadTaskCreate</docanchor>
    <docanchor file="event_records">ThreadReleaseLock_acquisitionOrder</docanchor>
    <docanchor file="event_records">ThreadFork_model</docanchor>
    <docanchor file="event_records">RmaCollectiveEnd_bytesReceived</docanchor>
    <docanchor file="event_records">ParameterInt</docanchor>
    <docanchor file="event_records">OmpTaskSwitch_taskID</docanchor>
    <docanchor file="event_records">OmpAcquireLock_lockID</docanchor>
    <docanchor file="event_records">OmpAcquireLock</docanchor>
    <docanchor file="event_records">MpiCollectiveEnd_collectiveOp</docanchor>
    <docanchor file="event_records">MpiRequestTest_requestID</docanchor>
    <docanchor file="event_records">Enter_region</docanchor>
    <docanchor file="event_records">ThreadTaskComplete_creatingThread</docanchor>
    <docanchor file="event_records">ThreadTaskCreate_creatingThread</docanchor>
    <docanchor file="event_records">ThreadTeamEnd</docanchor>
    <docanchor file="event_records">RmaOpCompleteNonBlocking</docanchor>
    <docanchor file="event_records">RmaAtomic_bytesSent</docanchor>
    <docanchor file="event_records">RmaReleaseLock_remote</docanchor>
    <docanchor file="event_records">RmaTryLock_lockType</docanchor>
    <docanchor file="event_records">RmaAcquireLock_lockType</docanchor>
    <docanchor file="event_records">RmaRequestLock_win</docanchor>
    <docanchor file="event_records">RmaCollectiveEnd</docanchor>
    <docanchor file="event_records">ParameterInt_parameter</docanchor>
    <docanchor file="event_records">OmpTaskComplete_taskID</docanchor>
    <docanchor file="event_records">OmpTaskComplete</docanchor>
    <docanchor file="event_records">BufferFlush</docanchor>
    <docanchor file="event_records">ThreadTaskSwitch_creatingThread</docanchor>
    <docanchor file="event_records">RmaOpCompleteBlocking_win</docanchor>
    <docanchor file="event_records">RmaAtomic_bytesReceived</docanchor>
    <docanchor file="event_records">RmaGet_remote</docanchor>
    <docanchor file="event_records">RmaPut_win</docanchor>
    <docanchor file="event_records">RmaTryLock_lockId</docanchor>
    <docanchor file="event_records">RmaCollectiveEnd_win</docanchor>
    <docanchor file="event_records">RmaCollectiveEnd_syncLevel</docanchor>
    <docanchor file="event_records">RmaWinDestroy</docanchor>
    <docanchor file="event_records">MpiCollectiveEnd_root</docanchor>
    <docanchor file="event_records">MpiIsendComplete_requestID</docanchor>
    <docanchor file="event_records">ThreadTaskComplete_threadTeam</docanchor>
    <docanchor file="event_records">ThreadTaskComplete</docanchor>
    <docanchor file="event_records">ThreadFork_numberOfRequestedThreads</docanchor>
    <docanchor file="event_records">ThreadFork</docanchor>
    <docanchor file="event_records">RmaOpCompleteBlocking</docanchor>
    <docanchor file="event_records">RmaReleaseLock_lockId</docanchor>
    <docanchor file="event_records">RmaGroupSync_syncLevel</docanchor>
    <docanchor file="event_records">Metric_numberOfMetrics</docanchor>
    <docanchor file="event_records">OmpReleaseLock</docanchor>
    <docanchor file="event_records">MpiRequestCancelled_requestID</docanchor>
    <docanchor file="event_records">MpiIrecv_requestID</docanchor>
    <docanchor file="event_records">MpiSend_msgLength</docanchor>
    <docanchor file="event_records">ThreadReleaseLock</docanchor>
    <docanchor file="event_records">RmaWaitChange</docanchor>
    <docanchor file="event_records">RmaRequestLock</docanchor>
    <docanchor file="event_records">ParameterUnsignedInt_value</docanchor>
    <docanchor file="event_records">ParameterUnsignedInt</docanchor>
    <docanchor file="event_records">ParameterString</docanchor>
    <docanchor file="event_records">Metric_typeIDs</docanchor>
    <docanchor file="event_records">OmpTaskSwitch</docanchor>
    <docanchor file="event_records">OmpFork</docanchor>
    <docanchor file="event_records">MpiIrecv_communicator</docanchor>
    <docanchor file="event_records">MpiIrecv_sender</docanchor>
    <docanchor file="event_records">MpiIsend_msgLength</docanchor>
    <docanchor file="event_records">MeasurementOnOff_measurementMode</docanchor>
    <docanchor file="event_records">ThreadTaskSwitch_generationNumber</docanchor>
    <docanchor file="event_records">ThreadTaskSwitch_threadTeam</docanchor>
    <docanchor file="event_records">ThreadTeamBegin</docanchor>
    <docanchor file="event_records">ThreadJoin_model</docanchor>
    <docanchor file="event_records">RmaOpTest_matchingId</docanchor>
    <docanchor file="event_records">RmaGroupSync_group</docanchor>
    <docanchor file="event_records">RmaGroupSync</docanchor>
    <docanchor file="event_records">RmaCollectiveEnd_collectiveOp</docanchor>
    <docanchor file="event_records">RmaCollectiveBegin</docanchor>
    <docanchor file="event_records">Metric_metricValues</docanchor>
    <docanchor file="event_records">OmpJoin</docanchor>
    <docanchor file="event_records">MpiRequestCancelled</docanchor>
    <docanchor file="event_records">MpiSend_receiver</docanchor>
    <docanchor file="event_records">ThreadAcquireLock_lockID</docanchor>
    <docanchor file="event_records">RmaOpCompleteNonBlocking_win</docanchor>
    <docanchor file="event_records">RmaGet</docanchor>
    <docanchor file="event_records">RmaAcquireLock_remote</docanchor>
    <docanchor file="event_records">RmaCollectiveEnd_root</docanchor>
    <docanchor file="event_records">Metric</docanchor>
    <docanchor file="event_records">MpiCollectiveBegin</docanchor>
    <docanchor file="event_records">MpiIrecv_msgLength</docanchor>
    <docanchor file="event_records">MpiIsend_msgTag</docanchor>
  </compound>
  <compound kind="page">
    <name>snap_records</name>
    <title>List of all snapshot records</title>
    <filename>snap_records</filename>
    <docanchor file="snap_records">ParameterUnsignedIntSnap_value</docanchor>
    <docanchor file="snap_records">MetricSnap</docanchor>
    <docanchor file="snap_records">OmpTaskCreateSnap_origEventTime</docanchor>
    <docanchor file="snap_records">MpiSendSnap</docanchor>
    <docanchor file="snap_records">MpiRecvSnap_sender</docanchor>
    <docanchor file="snap_records">MpiRecvSnap_origEventTime</docanchor>
    <docanchor file="snap_records">MpiIsendCompleteSnap_origEventTime</docanchor>
    <docanchor file="snap_records">MpiIsendSnap_msgLength</docanchor>
    <docanchor file="snap_records">ParameterIntSnap_value</docanchor>
    <docanchor file="snap_records">ParameterIntSnap_parameter</docanchor>
    <docanchor file="snap_records">MpiCollectiveEndSnap_sizeSent</docanchor>
    <docanchor file="snap_records">MpiCollectiveBeginSnap_origEventTime</docanchor>
    <docanchor file="snap_records">MpiIrecvSnap_msgTag</docanchor>
    <docanchor file="snap_records">SnapshotEnd</docanchor>
    <docanchor file="snap_records">MetricSnap_numberOfMetrics</docanchor>
    <docanchor file="snap_records">MpiIrecvSnap_sender</docanchor>
    <docanchor file="snap_records">MpiSendSnap_msgTag</docanchor>
    <docanchor file="snap_records">MpiSendSnap_communicator</docanchor>
    <docanchor file="snap_records">EnterSnap</docanchor>
    <docanchor file="snap_records">MetricSnap_typeIDs</docanchor>
    <docanchor file="snap_records">OmpForkSnap_origEventTime</docanchor>
    <docanchor file="snap_records">MpiIrecvRequestSnap_origEventTime</docanchor>
    <docanchor file="snap_records">SnapshotEnd_contReadPos</docanchor>
    <docanchor file="snap_records">ParameterUnsignedIntSnap_parameter</docanchor>
    <docanchor file="snap_records">MetricSnap_origEventTime</docanchor>
    <docanchor file="snap_records">MpiCollectiveEndSnap_sizeReceived</docanchor>
    <docanchor file="snap_records">MpiCollectiveEndSnap_communicator</docanchor>
    <docanchor file="snap_records">MpiIrecvSnap_msgLength</docanchor>
    <docanchor file="snap_records">MpiRecvSnap_communicator</docanchor>
    <docanchor file="snap_records">MpiRecvSnap</docanchor>
    <docanchor file="snap_records">MpiIsendSnap_origEventTime</docanchor>
    <docanchor file="snap_records">MeasurementOnOffSnap</docanchor>
    <docanchor file="snap_records">SnapshotStart</docanchor>
    <docanchor file="snap_records">ParameterStringSnap_parameter</docanchor>
    <docanchor file="snap_records">OmpTaskCreateSnap</docanchor>
    <docanchor file="snap_records">OmpAcquireLockSnap_lockID</docanchor>
    <docanchor file="snap_records">MpiCollectiveEndSnap</docanchor>
    <docanchor file="snap_records">EnterSnap_region</docanchor>
    <docanchor file="snap_records">ParameterIntSnap</docanchor>
    <docanchor file="snap_records">OmpTaskCreateSnap_taskID</docanchor>
    <docanchor file="snap_records">OmpAcquireLockSnap_acquisitionOrder</docanchor>
    <docanchor file="snap_records">OmpForkSnap_numberOfRequestedThreads</docanchor>
    <docanchor file="snap_records">MpiCollectiveEndSnap_collectiveOp</docanchor>
    <docanchor file="snap_records">MpiCollectiveEndSnap_origEventTime</docanchor>
    <docanchor file="snap_records">MpiIrecvSnap_requestID</docanchor>
    <docanchor file="snap_records">MpiIsendCompleteSnap_requestID</docanchor>
    <docanchor file="snap_records">MpiIsendSnap_requestID</docanchor>
    <docanchor file="snap_records">SnapshotStart_numberOfRecords</docanchor>
    <docanchor file="snap_records">ParameterUnsignedIntSnap_origEventTime</docanchor>
    <docanchor file="snap_records">ParameterStringSnap_string</docanchor>
    <docanchor file="snap_records">OmpAcquireLockSnap</docanchor>
    <docanchor file="snap_records">MpiIsendSnap_receiver</docanchor>
    <docanchor file="snap_records">MpiIsendSnap</docanchor>
    <docanchor file="snap_records">MpiSendSnap_origEventTime</docanchor>
    <docanchor file="snap_records">MetricSnap_metricValues</docanchor>
    <docanchor file="snap_records">OmpTaskSwitchSnap_taskID</docanchor>
    <docanchor file="snap_records">MpiIrecvSnap</docanchor>
    <docanchor file="snap_records">MpiRecvSnap_msgTag</docanchor>
    <docanchor file="snap_records">MpiSendSnap_msgLength</docanchor>
    <docanchor file="snap_records">ParameterIntSnap_origEventTime</docanchor>
    <docanchor file="snap_records">MeasurementOnOffSnap_measurementMode</docanchor>
    <docanchor file="snap_records">MpiCollectiveEndSnap_root</docanchor>
    <docanchor file="snap_records">MpiIrecvRequestSnap_requestID</docanchor>
    <docanchor file="snap_records">MpiIsendCompleteSnap</docanchor>
    <docanchor file="snap_records">MpiIsendSnap_msgTag</docanchor>
    <docanchor file="snap_records">MpiIsendSnap_communicator</docanchor>
    <docanchor file="snap_records">EnterSnap_origEventTime</docanchor>
    <docanchor file="snap_records">OmpTaskSwitchSnap</docanchor>
    <docanchor file="snap_records">MpiIrecvRequestSnap</docanchor>
    <docanchor file="snap_records">MpiSendSnap_receiver</docanchor>
    <docanchor file="snap_records">OmpAcquireLockSnap_origEventTime</docanchor>
    <docanchor file="snap_records">OmpForkSnap</docanchor>
    <docanchor file="snap_records">ParameterStringSnap_origEventTime</docanchor>
    <docanchor file="snap_records">MetricSnap_metric</docanchor>
    <docanchor file="snap_records">OmpTaskSwitchSnap_origEventTime</docanchor>
    <docanchor file="snap_records">MeasurementOnOffSnap_origEventTime</docanchor>
    <docanchor file="snap_records">ParameterUnsignedIntSnap</docanchor>
    <docanchor file="snap_records">ParameterStringSnap</docanchor>
    <docanchor file="snap_records">MpiIrecvSnap_communicator</docanchor>
    <docanchor file="snap_records">MpiIrecvSnap_origEventTime</docanchor>
    <docanchor file="snap_records">MpiRecvSnap_msgLength</docanchor>
    <docanchor file="snap_records">MpiCollectiveBeginSnap</docanchor>
  </compound>
  <compound kind="page">
    <name>usage_writing_page</name>
    <title>Usage in writing mode</title>
    <filename>usage_writing_page</filename>
    <docanchor file="usage_writing_page">usage_writing</docanchor>
  </compound>
  <compound kind="page">
    <name>usage_reading_page</name>
    <title>Usage in reading mode</title>
    <filename>usage_reading_page</filename>
    <docanchor file="usage_reading_page">usage_reading</docanchor>
  </compound>
  <compound kind="union">
    <name>OTF2_AttributeValue</name>
    <filename>unionOTF2__AttributeValue.html</filename>
    <member kind="variable">
      <type>OTF2_AttributeRef</type>
      <name>attributeRef</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>a351dcb2d039d12f878e183a5ba5870a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OTF2_CommRef</type>
      <name>commRef</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>a227cf53e11e697d68ca9e2ab8201fe00</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>float32</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>a9d2de3ffbd912be6ceee6b79a0f27855</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>float64</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>a23896491c45655ab559bff387d0c3a35</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OTF2_GroupRef</type>
      <name>groupRef</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>ab04b8184a8b0b72ab067ddc5a3a7cd02</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int16_t</type>
      <name>int16</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>a2a7d15a3b138e8df73a80e9faa111d94</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int32_t</type>
      <name>int32</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>a57f4845117aa455f89d30538bbdbc65b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int64_t</type>
      <name>int64</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>adbad140f6f5d3313bdbef0af921a1dd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int8_t</type>
      <name>int8</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>abf02cc1c57f0857f11a105750105e09f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OTF2_LocationRef</type>
      <name>locationRef</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>af1afb1a8fa1df9a693a3f19da884268e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OTF2_MetricRef</type>
      <name>metricRef</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>a47c3f0539ddb8a0caf955ebdaf63429b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OTF2_ParameterRef</type>
      <name>parameterRef</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>a58bbfce3e97ef3892eb7ff060ec74d11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OTF2_RegionRef</type>
      <name>regionRef</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>aaeaa407dd3f2fd1aea8076d1f68c90c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OTF2_RmaWinRef</type>
      <name>rmaWinRef</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>a4b396b32c2afd1b2e98523b91eb12f67</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OTF2_StringRef</type>
      <name>stringRef</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>aa80cb99039d8c5d5e8f581c6339814ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>uint16</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>af34883ba7e2e3263981f3619ea81716d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>uint32</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>a4c67232ca95b52240d01509cfdd52663</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>uint64</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>a46af35004cffc43c92ceb22d56e79df8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>uint8</name>
      <anchorfile>unionOTF2__AttributeValue.html</anchorfile>
      <anchor>aacff9926811eb33697e8a39800380dba</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>OTF2_FileSionCallbacks</name>
    <filename>structOTF2__FileSionCallbacks.html</filename>
    <member kind="variable">
      <type>OTF2_FileSionClose</type>
      <name>otf2_file_sion_close</name>
      <anchorfile>structOTF2__FileSionCallbacks.html</anchorfile>
      <anchor>a8a0f1ce75d4aca26cb90d030ef4766ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OTF2_FileSionGetRank</type>
      <name>otf2_file_sion_get_rank</name>
      <anchorfile>structOTF2__FileSionCallbacks.html</anchorfile>
      <anchor>a73c4e138c3eccf3296285a52bf419a62</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OTF2_FileSionOpen</type>
      <name>otf2_file_sion_open</name>
      <anchorfile>structOTF2__FileSionCallbacks.html</anchorfile>
      <anchor>a7abdd34d6cf57f1c23bfd0d228de6ec0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>OTF2_FlushCallbacks</name>
    <filename>structOTF2__FlushCallbacks.html</filename>
    <member kind="variable">
      <type>OTF2_PostFlushCallback</type>
      <name>otf2_post_flush</name>
      <anchorfile>structOTF2__FlushCallbacks.html</anchorfile>
      <anchor>a5a90fca1e02d67a3c2fca21dd76db45f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OTF2_PreFlushCallback</type>
      <name>otf2_pre_flush</name>
      <anchorfile>structOTF2__FlushCallbacks.html</anchorfile>
      <anchor>a82a8efae7b22c019fda1c31d886c00be</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>OTF2_MemoryCallbacks</name>
    <filename>structOTF2__MemoryCallbacks.html</filename>
    <member kind="variable">
      <type>OTF2_MemoryAllocate</type>
      <name>otf2_allocate</name>
      <anchorfile>structOTF2__MemoryCallbacks.html</anchorfile>
      <anchor>ae0e7291dfed047c4ee58fe42ea242412</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OTF2_MemoryFreeAll</type>
      <name>otf2_free_all</name>
      <anchorfile>structOTF2__MemoryCallbacks.html</anchorfile>
      <anchor>a0c655034dddfe0e21f3be0a362d8d229</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="union">
    <name>OTF2_MetricValue_union</name>
    <filename>unionOTF2__MetricValue__union.html</filename>
  </compound>
</tagfile>
