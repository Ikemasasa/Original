#pragma once

class IFieldUseItemEffect
{
protected:
	bool mIsEffectFinished = false;

public:
	virtual void Execute() = 0;
	bool IsEffectFinished() const { return mIsEffectFinished; }
};

class SubDueProofEffect : public IFieldUseItemEffect
{
public:
	void Execute() override;
};