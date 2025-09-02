// All copyrights for this code are owned by Aster.


#include "PersonalityCalculator.h"
#include "ProjectPA/Interfaces/PersonalityTraitInteractable.h"

bool UPersonalityCalculator::EvaluateComparison(TScriptInterface<IPersonalityTraitInteractable> TargetObject, const FPersonalityOperator& Operator) const
{
	TOptional<int32> ValueA = GetValueFromTarget(TargetObject, Operator.VariableName, Operator.AttributeContainerType);
	if (ValueA.IsSet() == false)
	{
		return false;
	}

	TOptional<int32> ValueB = 0.0f;
	if (Operator.bUseFixedValue)
	{
		ValueB = Operator.FixedValue;
	}
	else
	{
		ValueB = GetValueFromTarget(TargetObject, Operator.OtherVariableName, Operator.AttributeContainerType);
	}

	if (ValueB.IsSet() == false)
	{
		return false;
	}

	switch (Operator.ComparisonOperator)
	{
	case EComparisonOperator::EqualTo: return ValueA.GetValue() == ValueB.GetValue();
	case EComparisonOperator::NotEqualTo: return ValueA.GetValue() != ValueB.GetValue();
	case EComparisonOperator::GreaterThan: return ValueA.GetValue() > ValueB.GetValue();
	case EComparisonOperator::LessThan: return ValueA.GetValue() < ValueB.GetValue();
	case EComparisonOperator::GreaterThanOrEqualTo: return ValueA.GetValue() >= ValueB.GetValue();
	case EComparisonOperator::LessThanOrEqualTo: return ValueA.GetValue() <= ValueB.GetValue();
	default: return false;
	}
}

TOptional<int32> UPersonalityCalculator::GetValueFromTarget(const TScriptInterface<IPersonalityTraitInteractable>& TargetObject, const FName& VariableName, const
	EAttributeContainerType ContainerType)
{
	UObject* Target = TargetObject.GetObject();
	if (!Target)
	{
		return {};
	}
	
	FProperty* FoundProperty = Target->GetClass()->FindPropertyByName(GetContainerName(ContainerType));
	if (FoundProperty && FoundProperty->IsA(FStructProperty::StaticClass()))
	{
		FStructProperty* StructProp = CastField<FStructProperty>(FoundProperty);
		if (StructProp->Struct == GetScriptStruct(ContainerType))
		{
			void* StructPtr = StructProp->ContainerPtrToValuePtr<void>(Target);
			const FIntProperty* IntProp = FindFProperty<FIntProperty>(StructProp->Struct, VariableName);
			if (IntProp)
			{
				return IntProp->GetPropertyValue_InContainer(StructPtr);
			}
		}
	}

	return {};
}

FName UPersonalityCalculator::GetContainerName(const EAttributeContainerType ContainerType)
{
	switch (ContainerType)
	{
	case EAttributeContainerType::Personality :
		return TEXT("Personality");
	case EAttributeContainerType::Relationships :
		return TEXT("ToPlayerRelationships");
	}
	return TEXT("");
}

UScriptStruct* UPersonalityCalculator::GetScriptStruct(const EAttributeContainerType ContainerType)
{
	switch (ContainerType)
	{
	case EAttributeContainerType::Personality :
		return FPersonalityTraits::StaticStruct();
	case EAttributeContainerType::Relationships :
		return FToPlayerRelationships::StaticStruct();
	}
	return nullptr;
}
