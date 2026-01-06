// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
UCLASS()
class QUICKTWEENVISUALTOOL_API UQuickTweenEdGraph : public UEdGraph
{
	GENERATED_BODY()
public:
	virtual void GetGraphContextActions(FGraphActionMenuBuilder& contextMenuBuilder) const override;

	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* a, const UEdGraphPin* b) const override;
};
