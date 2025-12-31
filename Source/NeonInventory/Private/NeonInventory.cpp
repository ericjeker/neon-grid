#include "NeonInventory.h"

#define LOCTEXT_NAMESPACE "FNeonInventoryModule"

void FNeonInventoryModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("NeonInventoryModule: Module Started!"));
}

void FNeonInventoryModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FNeonInventoryModule, NeonInventory)