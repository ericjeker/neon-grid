#include "Misc/AutomationTest.h"
#include "NeonGrid/Components/PatrolPointsComponent.h"

// Parameters: (ClassName, "Pretty.Path.In.Test.UI", Flags)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPatrolPointsComponentTest, "NeonGrid.Components.PatrolPointsComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FPatrolPointsComponentTest::RunTest(const FString& Parameters)
{
	// Create a test component
	UPatrolPointsComponent* PatrolComponent = NewObject<UPatrolPointsComponent>();
	
	// Test 1: Empty patrol points should return zero vector
	int32 Index = 0;
	bool bIsReversing = false;
	FVector Result = PatrolComponent->GetNextPatrolPoint(Index, bIsReversing);
	TestEqual("Empty patrol points returns zero vector", Result, FVector::ZeroVector);
	
	// Test 2: Sequential mode cycles through points
	TArray<FVector> TestPoints = {
		FVector(0, 0, 0),
		FVector(100, 0, 0),
		FVector(100, 100, 0)
	};
	
	// Use reflection to set the patrol points (since it's protected)
	const FProperty* PatrolPointsProperty = PatrolComponent->GetClass()->FindPropertyByName(TEXT("PatrolPoints"));
	if (PatrolPointsProperty)
	{
		void* PropertyValuePtr = PatrolPointsProperty->ContainerPtrToValuePtr<void>(PatrolComponent);
		PatrolPointsProperty->CopyCompleteValue(PropertyValuePtr, &TestPoints);
	}
	
	Index = 0;
	PatrolComponent->GetNextPatrolPoint(Index, bIsReversing);
	TestEqual("Sequential mode moves to next index", Index, 1);
	
	PatrolComponent->GetNextPatrolPoint(Index, bIsReversing);
	TestEqual("Sequential mode continues to next index", Index, 2);
	
	PatrolComponent->GetNextPatrolPoint(Index, bIsReversing);
	TestEqual("Sequential mode wraps around to start", Index, 0);
	
	// Test 3: Patrol point count
	TestEqual("GetPatrolPointCount returns correct count", PatrolComponent->GetPatrolPointCount(), 3);
	
	// Test 4: GetPatrolPointWorldLocation with an invalid index
	FVector InvalidResult = PatrolComponent->GetPatrolPointWorldLocation(99);
	TestEqual("Invalid index returns zero vector", InvalidResult, FVector::ZeroVector);

	return true;
}
