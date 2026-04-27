// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomNavigationConfig.h"

FCustomNavigationConfig::FCustomNavigationConfig()
{
	KeyEventRules.Emplace(EKeys::A, EUINavigation::Left);
	KeyEventRules.Emplace(EKeys::D, EUINavigation::Right);
	KeyEventRules.Emplace(EKeys::W, EUINavigation::Up);
	KeyEventRules.Emplace(EKeys::S, EUINavigation::Down);
}
