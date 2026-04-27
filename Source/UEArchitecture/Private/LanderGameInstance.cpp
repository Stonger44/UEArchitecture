// Fill out your copyright notice in the Description page of Project Settings.


#include "LanderGameInstance.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/NavigationConfig.h"
#include "CustomNavigationConfig.h"


void ULanderGameInstance::Init()
{
	Super::Init();

	FSlateApplication& SlateApp = FSlateApplication::Get();
	SlateApp.SetNavigationConfig(MakeShared<FCustomNavigationConfig>());
}