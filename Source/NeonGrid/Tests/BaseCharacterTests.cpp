#include "Misc/AutomationTest.h"

// Parameters: (ClassName, "Pretty.Path.In.Test.UI", Flags)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBaseCharacterTest, "NeonGrid.Characters.BaseCharacterBasicTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBaseCharacterTest::RunTest(const FString& Parameters)
{
	// Your test logic goes here
    
	// Example: Test a simple condition
	int32 Health = 100;
	TestEqual(TEXT("Health should be 100 at start"), Health, 100);

	// If you return true, the test passes (provided no TestErrors occurred)
	return true;
}