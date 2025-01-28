// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Virtual_life_project/Public/visible_wall_component.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodevisible_wall_component() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
UPackage* Z_Construct_UPackage__Script_Virtual_life_project();
VIRTUAL_LIFE_PROJECT_API UClass* Z_Construct_UClass_Uvisible_wall_component();
VIRTUAL_LIFE_PROJECT_API UClass* Z_Construct_UClass_Uvisible_wall_component_NoRegister();
// End Cross Module References

// Begin Class Uvisible_wall_component
void Uvisible_wall_component::StaticRegisterNativesUvisible_wall_component()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(Uvisible_wall_component);
UClass* Z_Construct_UClass_Uvisible_wall_component_NoRegister()
{
	return Uvisible_wall_component::StaticClass();
}
struct Z_Construct_UClass_Uvisible_wall_component_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "visible_wall_component.h" },
		{ "ModuleRelativePath", "Public/visible_wall_component.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<Uvisible_wall_component>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_Uvisible_wall_component_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_Virtual_life_project,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_Uvisible_wall_component_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_Uvisible_wall_component_Statics::ClassParams = {
	&Uvisible_wall_component::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_Uvisible_wall_component_Statics::Class_MetaDataParams), Z_Construct_UClass_Uvisible_wall_component_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_Uvisible_wall_component()
{
	if (!Z_Registration_Info_UClass_Uvisible_wall_component.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_Uvisible_wall_component.OuterSingleton, Z_Construct_UClass_Uvisible_wall_component_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_Uvisible_wall_component.OuterSingleton;
}
template<> VIRTUAL_LIFE_PROJECT_API UClass* StaticClass<Uvisible_wall_component>()
{
	return Uvisible_wall_component::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(Uvisible_wall_component);
Uvisible_wall_component::~Uvisible_wall_component() {}
// End Class Uvisible_wall_component

// Begin Registration
struct Z_CompiledInDeferFile_FID_minji_UnrealEngine_VirtualLife_Virtual_life_project_Source_Virtual_life_project_Public_visible_wall_component_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_Uvisible_wall_component, Uvisible_wall_component::StaticClass, TEXT("Uvisible_wall_component"), &Z_Registration_Info_UClass_Uvisible_wall_component, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(Uvisible_wall_component), 3798291900U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_minji_UnrealEngine_VirtualLife_Virtual_life_project_Source_Virtual_life_project_Public_visible_wall_component_h_1454276347(TEXT("/Script/Virtual_life_project"),
	Z_CompiledInDeferFile_FID_minji_UnrealEngine_VirtualLife_Virtual_life_project_Source_Virtual_life_project_Public_visible_wall_component_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_minji_UnrealEngine_VirtualLife_Virtual_life_project_Source_Virtual_life_project_Public_visible_wall_component_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
