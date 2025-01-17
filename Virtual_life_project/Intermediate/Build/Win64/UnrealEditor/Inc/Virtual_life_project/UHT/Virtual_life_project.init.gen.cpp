// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVirtual_life_project_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_Virtual_life_project;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_Virtual_life_project()
	{
		if (!Z_Registration_Info_UPackage__Script_Virtual_life_project.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/Virtual_life_project",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x239AEF63,
				0x51AD710F,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_Virtual_life_project.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_Virtual_life_project.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_Virtual_life_project(Z_Construct_UPackage__Script_Virtual_life_project, TEXT("/Script/Virtual_life_project"), Z_Registration_Info_UPackage__Script_Virtual_life_project, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x239AEF63, 0x51AD710F));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
