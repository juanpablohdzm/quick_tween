// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.
#include "SQuickTweenEditor.h"

void SQuickTweenEditor::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(STextBlock)
		.Text(FText::FromString("Quick Tween Graph Editor"))
	];
}
