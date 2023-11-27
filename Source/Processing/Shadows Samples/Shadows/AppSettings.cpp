#include "PCH.h"
#include "AppSettings.h"

using namespace SampleFramework11;

static const char* SceneLabels[3] =
{
    "PowerPlant",
    "Tower",
    "Columns",
};

static const char* ShadowModeLabels[7] =
{
    "Fixed Size PCF",
    "Grid PCF",
    "Random Disc PCF",
    "Optimized PCF",
    "VSM",
    "EVSM 2 Component",
    "EVSM 4 Component",
};

static const char* ShadowMapSizeLabels[3] =
{
    "512x512",
    "1024x1024",
    "2048x2048",
};

static const char* FixedFilterSizeLabels[5] =
{
    "2x2",
    "3x3",
    "5x5",
    "7x7",
    "9x9",
};

static const char* PartitionModeLabels[3] =
{
    "Manual",
    "Logarithmic",
    "PSSM",
};

static const char* ShadowMSAALabels[4] =
{
    "None",
    "2x",
    "4x",
    "8x",
};

static const char* VSMFormatLabels[2] =
{
    "16-bit",
    "32-bit",
};

static const char* ShadowAnisotropyLabels[5] =
{
    "1x",
    "2x",
    "4x",
    "8x",
    "16x",
};

namespace AppSettings
{
    SceneSetting CurrentScene;
    BoolSetting AnimateLight;
    DirectionSetting LightDirection;
    ColorSetting LightColor;
    OrientationSetting CharacterOrientation;
    BoolSetting EnableAlbedoMap;
    ShadowModeSetting ShadowMode;
    ShadowMapSizeSetting ShadowMapSize;
    FixedFilterSizeSetting FixedFilterSize;
    FloatSetting FilterSize;
    BoolSetting VisualizeCascades;
    BoolSetting StabilizeCascades;
    BoolSetting FilterAcrossCascades;
    BoolSetting RandomizeDiscOffsets;
    IntSetting NumDiscSamples;
    BoolSetting AutoComputeDepthBounds;
    IntSetting ReadbackLatency;
    BoolSetting GPUSceneSubmission;
    FloatSetting MinCascadeDistance;
    FloatSetting MaxCascadeDistance;
    PartitionModeSetting PartitionMode;
    FloatSetting SplitDistance0;
    FloatSetting SplitDistance1;
    FloatSetting SplitDistance2;
    FloatSetting SplitDistance3;
    FloatSetting PSSMLambda;
    BoolSetting UsePlaneDepthBias;
    FloatSetting Bias;
    FloatSetting OffsetScale;
    ShadowMSAASetting ShadowMSAA;
    VSMFormatSetting VSMFormat;
    ShadowAnisotropySetting ShadowAnisotropy;
    BoolSetting EnableShadowMips;
    FloatSetting PositiveExponent;
    FloatSetting NegativeExponent;
    FloatSetting LightBleedingReduction;
    FloatSetting BloomThreshold;
    FloatSetting BloomMagnitude;
    FloatSetting BloomBlurSigma;
    FloatSetting KeyValue;
    FloatSetting AdaptationRate;

    ConstantBuffer<AppSettingsCBuffer> CBuffer;

    void Initialize(ID3D11Device* device)
    {
        TwBar* tweakBar = Settings.TweakBar();

        CurrentScene.Initialize(tweakBar, "CurrentScene", "SceneControls", "Current Scene", "The scene to render", Scene::PowerPlant, 3, SceneLabels);
        Settings.AddSetting(&CurrentScene);

        AnimateLight.Initialize(tweakBar, "AnimateLight", "SceneControls", "Animate Light", "Automatically rotates the light about the Y axis", false);
        Settings.AddSetting(&AnimateLight);

        LightDirection.Initialize(tweakBar, "LightDirection", "SceneControls", "Light Direction", "The direction of the light", Float3(1.0000f, 1.0000f, 1.0000f));
        Settings.AddSetting(&LightDirection);

        LightColor.Initialize(tweakBar, "LightColor", "SceneControls", "Light Color", "The color of the light", Float3(10.0000f, 8.0000f, 5.0000f), true, 0.0000f, 20.0000f, 0.1000f);
        Settings.AddSetting(&LightColor);

        CharacterOrientation.Initialize(tweakBar, "CharacterOrientation", "SceneControls", "Character Orientation", "The orientation of the character model", Quaternion(0.0000f, 0.0000f, 0.0000f, 0.0000f));
        Settings.AddSetting(&CharacterOrientation);

        EnableAlbedoMap.Initialize(tweakBar, "EnableAlbedoMap", "SceneControls", "Enable Albedo Map", "Enables using albedo maps when rendering the scene", true);
        Settings.AddSetting(&EnableAlbedoMap);

        ShadowMode.Initialize(tweakBar, "ShadowMode", "Shadows", "Shadow Mode", "The shadow mapping technique to use", ShadowMode::FixedSizePCF, 7, ShadowModeLabels);
        Settings.AddSetting(&ShadowMode);

        ShadowMapSize.Initialize(tweakBar, "ShadowMapSize", "Shadows", "Shadow Map Size", "The size of the shadow map", ShadowMapSize::SMSize2048, 3, ShadowMapSizeLabels);
        Settings.AddSetting(&ShadowMapSize);

        FixedFilterSize.Initialize(tweakBar, "FixedFilterSize", "Shadows", "Fixed Filter Size", "Size of the PCF kernel used for Fixed Sized PCF shadow mode", FixedFilterSize::Filter2x2, 5, FixedFilterSizeLabels);
        Settings.AddSetting(&FixedFilterSize);

        FilterSize.Initialize(tweakBar, "FilterSize", "Shadows", "Filter Size", "Width of the filter kernel used for PCF or VSM filtering", 0.0000f, 0.0000f, 100.0000f, 0.1000f);
        Settings.AddSetting(&FilterSize);

        VisualizeCascades.Initialize(tweakBar, "VisualizeCascades", "Shadows", "Visualize Cascades", "Colors each cascade a different color to visualize their start and end points", false);
        Settings.AddSetting(&VisualizeCascades);

        StabilizeCascades.Initialize(tweakBar, "StabilizeCascades", "Shadows", "Stabilize Cascades", "Keeps consistent sizes for each cascade, and snaps each cascade so that they move in texel-sized increments. Reduces temporal aliasing artifacts, but reduces the effective resolution of the cascades", false);
        Settings.AddSetting(&StabilizeCascades);

        FilterAcrossCascades.Initialize(tweakBar, "FilterAcrossCascades", "Shadows", "Filter Across Cascades", "Enables blending across cascade boundaries to reduce the appearance of seams", false);
        Settings.AddSetting(&FilterAcrossCascades);

        RandomizeDiscOffsets.Initialize(tweakBar, "RandomizeDiscOffsets", "Shadows", "Randomize Disc Offsets", "Applies a per-pixel random rotation to the sample locations when using disc PCF", false);
        Settings.AddSetting(&RandomizeDiscOffsets);

        NumDiscSamples.Initialize(tweakBar, "NumDiscSamples", "Shadows", "Num Disc Samples", "Number of samples to take when using randomized disc PCF", 16, 1, 64);
        Settings.AddSetting(&NumDiscSamples);

        AutoComputeDepthBounds.Initialize(tweakBar, "AutoComputeDepthBounds", "Shadows", "Auto-Compute Depth Bounds", "Automatically fits the cascades to the min and max depth of the scene based on the contents of the depth buffer", false);
        Settings.AddSetting(&AutoComputeDepthBounds);

        ReadbackLatency.Initialize(tweakBar, "ReadbackLatency", "Shadows", "Depth Bounds Readback Latency", "Number of frames to wait before reading back the depth reduction results", 1, 0, 3);
        Settings.AddSetting(&ReadbackLatency);

        GPUSceneSubmission.Initialize(tweakBar, "GPUSceneSubmission", "Shadows", "GPU Scene Submission", "Uses compute shaders to handle shadow setup and mesh batching to minimize draw calls and avoid depth readback latency", false);
        Settings.AddSetting(&GPUSceneSubmission);

        MinCascadeDistance.Initialize(tweakBar, "MinCascadeDistance", "Shadows", "Min Cascade Distance", "The closest depth that is covered by the shadow cascades", 0.0000f, 0.0000f, 0.1000f, 0.0010f);
        Settings.AddSetting(&MinCascadeDistance);

        MaxCascadeDistance.Initialize(tweakBar, "MaxCascadeDistance", "Shadows", "Max Cascade Distance", "The furthest depth that is covered by the shadow cascades", 1.0000f, 0.0000f, 1.0000f, 0.0100f);
        Settings.AddSetting(&MaxCascadeDistance);

        PartitionMode.Initialize(tweakBar, "PartitionMode", "Shadows", "CSM Partition Model", "Controls how the viewable depth range is partitioned into cascades", PartitionMode::Manual, 3, PartitionModeLabels);
        Settings.AddSetting(&PartitionMode);

        SplitDistance0.Initialize(tweakBar, "SplitDistance0", "Shadows", "Split Distance 0", "Normalized distance to the end of the first cascade split", 0.0500f, 0.0000f, 1.0000f, 0.0100f);
        Settings.AddSetting(&SplitDistance0);

        SplitDistance1.Initialize(tweakBar, "SplitDistance1", "Shadows", "Split Distance 1", "Normalized distance to the end of the first cascade split", 0.1500f, 0.0000f, 1.0000f, 0.0100f);
        Settings.AddSetting(&SplitDistance1);

        SplitDistance2.Initialize(tweakBar, "SplitDistance2", "Shadows", "Split Distance 2", "Normalized distance to the end of the first cascade split", 0.5000f, 0.0000f, 1.0000f, 0.0100f);
        Settings.AddSetting(&SplitDistance2);

        SplitDistance3.Initialize(tweakBar, "SplitDistance3", "Shadows", "Split Distance 3", "Normalized distance to the end of the first cascade split", 1.0000f, 0.0000f, 1.0000f, 0.0100f);
        Settings.AddSetting(&SplitDistance3);

        PSSMLambda.Initialize(tweakBar, "PSSMLambda", "Shadows", "PSSM Lambda", "Lambda parameter used when PSSM mode is used for generated, blends between a linear and a logarithmic distribution", 1.0000f, 0.0000f, 1.0000f, 0.0100f);
        Settings.AddSetting(&PSSMLambda);

        UsePlaneDepthBias.Initialize(tweakBar, "UsePlaneDepthBias", "Shadows", "Use Receiver Plane Depth Bias", "Automatically computes a bias value based on the slope of the receiver", true);
        Settings.AddSetting(&UsePlaneDepthBias);

        Bias.Initialize(tweakBar, "Bias", "Shadows", "Bias", "Bias used for shadow map depth comparisons", 0.0050f, 0.0000f, 0.0100f, 0.0001f);
        Settings.AddSetting(&Bias);

        OffsetScale.Initialize(tweakBar, "OffsetScale", "Shadows", "Offset Scale", "Shadow receiver offset along the surface normal direction", 0.0000f, 0.0000f, 100.0000f, 0.1000f);
        Settings.AddSetting(&OffsetScale);

        ShadowMSAA.Initialize(tweakBar, "ShadowMSAA", "Shadows", "Shadow MSAA", "MSAA mode to use for VSM shadow maps", ShadowMSAA::MSAANone, 4, ShadowMSAALabels);
        Settings.AddSetting(&ShadowMSAA);

        VSMFormat.Initialize(tweakBar, "VSMFormat", "Shadows", "VSM Format", "Texture format to use for VSM shadow maps", VSMFormat::VSM32Bit, 2, VSMFormatLabels);
        Settings.AddSetting(&VSMFormat);

        ShadowAnisotropy.Initialize(tweakBar, "ShadowAnisotropy", "Shadows", "VSM Shadow Anisotropy", "Level of anisotropic filtering to use when sampling VSM shadow maps", ShadowAnisotropy::Anisotropy1x, 5, ShadowAnisotropyLabels);
        Settings.AddSetting(&ShadowAnisotropy);

        EnableShadowMips.Initialize(tweakBar, "EnableShadowMips", "Shadows", "Enable Shadow Mip Maps", "Generates mip maps when using Variance Shadow Maps", false);
        Settings.AddSetting(&EnableShadowMips);

        PositiveExponent.Initialize(tweakBar, "PositiveExponent", "Shadows", "EVSM Positive Exponent", "Exponent used for the positive EVSM warp", 40.0000f, 0.0000f, 100.0000f, 0.1000f);
        Settings.AddSetting(&PositiveExponent);

        NegativeExponent.Initialize(tweakBar, "NegativeExponent", "Shadows", "EVSM Negative Exponent", "Exponent used for the negative EVSM warp", 5.0000f, 0.0000f, 100.0000f, 0.1000f);
        Settings.AddSetting(&NegativeExponent);

        LightBleedingReduction.Initialize(tweakBar, "LightBleedingReduction", "Shadows", "Light Bleeding Reduction", "Light bleeding reduction factor for VSM and EVSM shadows", 0.0000f, 0.0000f, 1.0000f, 0.0100f);
        Settings.AddSetting(&LightBleedingReduction);

        BloomThreshold.Initialize(tweakBar, "BloomThreshold", "PostProcessing", "Bloom Exposure Offset", "Exposure offset applied to generate the input of the bloom pass", 3.0000f, 0.0000f, 20.0000f, 0.0100f);
        Settings.AddSetting(&BloomThreshold);

        BloomMagnitude.Initialize(tweakBar, "BloomMagnitude", "PostProcessing", "Bloom Magnitude", "Scale factor applied to the bloom results when combined with tone-mapped result", 1.0000f, 0.0000f, 2.0000f, 0.0100f);
        Settings.AddSetting(&BloomMagnitude);

        BloomBlurSigma.Initialize(tweakBar, "BloomBlurSigma", "PostProcessing", "Bloom Blur Sigma", "Sigma parameter of the Gaussian filter used in the bloom pass", 0.8000f, 0.5000f, 1.5000f, 0.0100f);
        Settings.AddSetting(&BloomBlurSigma);

        KeyValue.Initialize(tweakBar, "KeyValue", "PostProcessing", "Auto-Exposure Key Value", "Parameter that biases the result of the auto-exposure pass", 0.1150f, 0.0000f, 0.5000f, 0.0100f);
        Settings.AddSetting(&KeyValue);

        AdaptationRate.Initialize(tweakBar, "AdaptationRate", "PostProcessing", "Adaptation Rate", "Controls how quickly auto-exposure adapts to changes in scene brightness", 0.5000f, 0.0000f, 4.0000f, 0.0100f);
        Settings.AddSetting(&AdaptationRate);

        CBuffer.Initialize(device);
    }

    void UpdateCBuffer(ID3D11DeviceContext* context)
    {
        CBuffer.Data.LightDirection = LightDirection;
        CBuffer.Data.LightColor = LightColor;
        CBuffer.Data.EnableAlbedoMap = EnableAlbedoMap;
        CBuffer.Data.ShadowMapSize = ShadowMapSize;
        CBuffer.Data.FilterSize = FilterSize;
        CBuffer.Data.StabilizeCascades = StabilizeCascades;
        CBuffer.Data.NumDiscSamples = NumDiscSamples;
        CBuffer.Data.AutoComputeDepthBounds = AutoComputeDepthBounds;
        CBuffer.Data.MinCascadeDistance = MinCascadeDistance;
        CBuffer.Data.MaxCascadeDistance = MaxCascadeDistance;
        CBuffer.Data.PartitionMode = PartitionMode;
        CBuffer.Data.SplitDistance0 = SplitDistance0;
        CBuffer.Data.SplitDistance1 = SplitDistance1;
        CBuffer.Data.SplitDistance2 = SplitDistance2;
        CBuffer.Data.SplitDistance3 = SplitDistance3;
        CBuffer.Data.PSSMLambda = PSSMLambda;
        CBuffer.Data.Bias = Bias;
        CBuffer.Data.OffsetScale = OffsetScale;
        CBuffer.Data.PositiveExponent = PositiveExponent;
        CBuffer.Data.NegativeExponent = NegativeExponent;
        CBuffer.Data.LightBleedingReduction = LightBleedingReduction;
        CBuffer.Data.BloomThreshold = BloomThreshold;
        CBuffer.Data.BloomMagnitude = BloomMagnitude;
        CBuffer.Data.BloomBlurSigma = BloomBlurSigma;
        CBuffer.Data.KeyValue = KeyValue;
        CBuffer.Data.AdaptationRate = AdaptationRate;

        CBuffer.ApplyChanges(context);
        CBuffer.SetVS(context, 7);
        CBuffer.SetHS(context, 7);
        CBuffer.SetDS(context, 7);
        CBuffer.SetGS(context, 7);
        CBuffer.SetPS(context, 7);
        CBuffer.SetCS(context, 7);
    }
}

// ================================================================================================

namespace AppSettings
{
    void Update()
    {
        bool enableSplits = PartitionMode == PartitionMode::Manual;
        bool enableLambda = PartitionMode == PartitionMode::PSSM;
        bool enableVSM = UseVSM();
        bool enableEVSM = UseEVSM();
        bool enableMinMaxDepth = AutoComputeDepthBounds == false;

        SplitDistance0.SetEditable(enableSplits);
        SplitDistance1.SetEditable(enableSplits);
        SplitDistance2.SetEditable(enableSplits);
        SplitDistance3.SetEditable(enableSplits);
        PSSMLambda.SetEditable(enableLambda);
        PositiveExponent.SetEditable(enableEVSM);
        NegativeExponent.SetEditable(enableEVSM);
        LightBleedingReduction.SetEditable(enableVSM);
        FilterSize.SetEditable(ShadowMode != ShadowMode::FixedSizePCF && ShadowMode != ShadowMode::OptimizedPCF);
        ReadbackLatency.SetEditable(enableVSM == false && GPUSceneSubmission == false);
        Bias.SetEditable(UsePlaneDepthBias == false);
        NumDiscSamples.SetEditable(ShadowMode == ShadowMode::RandomDiscPCF);
        MinCascadeDistance.SetEditable(enableMinMaxDepth);
        MaxCascadeDistance.SetEditable(enableMinMaxDepth);
        ReadbackLatency.SetEditable(AutoComputeDepthBounds != 0 && GPUSceneSubmission == false);
        FixedFilterSize.SetEditable(ShadowMode == ShadowMode::FixedSizePCF || ShadowMode == ShadowMode::OptimizedPCF);
        ShadowMSAA.SetEditable(enableVSM);
        EnableShadowMips.SetEditable(enableVSM);
        ShadowAnisotropy.SetEditable(enableVSM);

        static float SavedMinDepth = 0.0f;
        static float SavedMaxDepth = 1.0f;
        if(AutoComputeDepthBounds == false && AutoComputeDepthBounds.Changed())
        {
            MinCascadeDistance.SetValue(SavedMinDepth);
            MaxCascadeDistance.SetValue(SavedMaxDepth);
        }
        else if(AutoComputeDepthBounds == false)
        {
            SavedMinDepth = MinCascadeDistance;
            SavedMaxDepth = MaxCascadeDistance;
        }
    }
}