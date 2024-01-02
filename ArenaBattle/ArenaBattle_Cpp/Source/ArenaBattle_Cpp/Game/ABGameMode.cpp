// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"


AABGameMode::AABGameMode()
{
	//// 클래스 정보는 _C 붙이기
	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(
	//	TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	//if (ThirdPersonClassRef.Class)
	//{
	//	DefaultPawnClass = ThirdPersonClassRef.Class;
	//}

	// 클래스 정보는 _C 붙이기
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(
		TEXT("/Script/ArenaBattle_Cpp.ABCharacterPlayer"));
	if (ThirdPersonClassRef.Class)
	{
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}

	// 디폴트 컨트롤러 설정
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerRef(
		TEXT("/Script/ArenaBattle_Cpp.ABPlayerController"));
	if (PlayerControllerRef.Class)
	{
		PlayerControllerClass = PlayerControllerRef.Class;
	}

}
