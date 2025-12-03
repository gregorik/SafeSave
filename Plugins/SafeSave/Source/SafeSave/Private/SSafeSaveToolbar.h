#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SSafeSaveToolbar : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSafeSaveToolbar) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// FIX: Removed Tick() override. We use ActiveTimer now.

	// The Logic Loop
	EActiveTimerReturnType UpdateState(double InCurrentTime, float InDeltaTime);

private:
	FReply OnClicked();

	const FSlateBrush* GetIcon() const;
	FText GetLabel() const;
	FSlateColor GetColor() const;
	FText GetTooltip() const;

	void CheckForUpdates();

	// State Flags
	bool bIsDirty;
	bool bIsConnected;
	bool bHasRemoteChanges; // USP Feature
};