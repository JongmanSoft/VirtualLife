// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Virtual_life_project/Virtual_life_projectGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVirtual_life_projectGameMode() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
UPackage* Z_Construct_UPackage__Script_Virtual_life_project();
VIRTUAL_LIFE_PROJECT_API UClass* Z_Construct_UClass_AVirtual_life_projectGameMode();
VIRTUAL_LIFE_PROJECT_API UClass* Z_Construct_UClass_AVirtual_life_projectGameMode_NoRegister();
// End Cross Module References

// Begin Class AVirtual_life_projectGameMode
void AVirtual_life_projectGameMode::StaticRegisterNativesAVirtual_life_projectGameMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AVirtual_life_projectGameMode);
UClass* Z_Construct_UClass_AVirtual_life_projectGameMode_NoRegister()
{
	return AVirtual_life_projectGameMode::StaticClass();
}
struct Z_Construct_UClass_AVirtual_life_projectGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "Virtual_life_projectGameMode.h" },
		{ "ModuleRelativePath", "Virtual_life_projectGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AVirtual_life_projectGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AVirtual_life_projectGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_Virtual_life_project,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AVirtual_life_projectGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AVirtual_life_projectGameMode_Statics::ClassParams = {
	&AVirtual_life_projectGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x008802ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AVirtual_life_projectGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_AVirtual_life_projectGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AVirtual_life_projectGameMode()
{
	if (!Z_Registration_Info_UClass_AVirtual_life_projectGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AVirtual_life_projectGameMode.OuterSingleton, Z_Construct_UClass_AVirtual_life_projectGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AVirtual_life_projectGameMode.OuterSingleton;
}
template<> VIRTUAL_LIFE_PROJECT_API UClass* StaticClass<AVirtual_life_projectGameMode>()
{
	return AVirtual_life_projectGameMode::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AVirtual_life_projectGameMode);
AVirtual_life_projectGameMode::~AVirtual_life_projectGameMode() {}
// End Class AVirtual_life_projectGameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_minji_UnrealProject_virtual_life_Virtual_life_project_Source_Virtual_life_project_Virtual_life_projectGameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AVirtual_life_projectGameMode, AVirtual_life_projectGameMode::StaticClass, TEXT("AVirtual_life_projectGameMode"), &Z_Registration_Info_UClass_AVirtual_life_projectGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AVirtual_life_projectGameMode), 4008414440U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_minji_UnrealProject_virtual_life_Virtual_life_project_Source_Virtual_life_project_Virtual_life_projectGameMode_h_1736232458(TEXT("/Script/Virtual_life_project"),
	Z_CompiledInDeferFile_FID_Users_minji_UnrealProject_virtual_life_Virtual_life_project_Source_Virtual_life_project_Virtual_life_projectGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_minji_UnrealProject_virtual_life_Virtual_life_project_Source_Virtual_life_project_Virtual_life_projectGameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
