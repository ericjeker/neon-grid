#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

UCLASS()
class NEONGRID_API AWaveManager : public AActor
{
	GENERATED_BODY()
    
public:    
	AWaveManager();

	UFUNCTION(BlueprintCallable, Category = "Wave System")
	void RegisterNPCDeath(AActor* DeadNPC);
    
	UFUNCTION(BlueprintCallable, Category = "Wave System")
	int32 GetAliveNPCCount() const { return AliveNPCs.Num(); }

	UFUNCTION(BlueprintNativeEvent, Category = "Wave System")
	void OnNPCDeath(AActor* DeadNPC);
    
protected:
	virtual void BeginPlay() override;

	void FindSpawnAndPatrolPoints();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
	int32 NPCsPerWave = 3;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
	TSubclassOf<AActor> NPCClass;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave System")
	FName SpawnPointTag = "NPCSpawnPoint";
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave System")
	FName PatrolPointTag = "NPCPatrolPoint";
    
	UPROPERTY(BlueprintReadOnly, Category = "Wave System")
	int32 CurrentWaveNumber = 0;
    
private:
	TArray<TWeakObjectPtr<AActor>> SpawnPoints;
	TArray<TWeakObjectPtr<AActor>> PatrolPoints;
	TArray<TWeakObjectPtr<AActor>> AliveNPCs;
    
	UFUNCTION()
	void CheckAndSpawnWave();
    
	UFUNCTION()
	void SpawnWave();
    
};