// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class QUICKTWEEN_API IQuickTweenable
{
public:
	virtual ~IQuickTweenable() = default;

	virtual void Update(float deltaTime);

	virtual bool GetIsPlaying() const;

	virtual bool GetIsCompleted() const;

	virtual bool GetIsPendingKill() const;

};
