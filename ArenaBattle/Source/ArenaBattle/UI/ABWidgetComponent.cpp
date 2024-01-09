// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
#include "ABUserWidget.h"

void UABWidgetComponent::InitWidget()
{
	Super::InitWidget(); // 등록된 위젯을 생성

	// 생성된 위젯 정보를 가져온다.
	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget());
	if (ABUserWidget)
	{   // 확장한 위젯 클래스에 Owner를 넘겨준다.
		ABUserWidget->SetOwningActor(GetOwner());
	}
}
