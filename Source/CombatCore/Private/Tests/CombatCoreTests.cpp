#include "Misc/AutomationTest.h"
#include "Combat/CombatAttributeComponent.h"
#include "CombatAttributeTestListener.h"
#if WITH_DEV_AUTOMATION_TESTS

namespace
{
    UCombatAttributeComponent* MakeTestAttributes()
    {
        UCombatAttributeComponent* Attributes = NewObject<UCombatAttributeComponent>();
        Attributes->ResetAttributes();
        return Attributes;
    }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCombatCoreDamageTest,
    "CombatCore.Attribute.Damage",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCombatCoreDamageTest::RunTest(const FString& /*Parameters*/)
{
    UCombatAttributeComponent* Attributes = MakeTestAttributes();

    const float ActualDamage = Attributes->ApplyDamage(30.f, nullptr);

    TestEqual(TEXT("Actual damage is 30"), ActualDamage, 30.f);
    TestEqual(TEXT("Health is 70"), Attributes->GetHealth(), 70.f);
    TestFalse(TEXT("Still alive"), Attributes->IsDead());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCombatCoreHealthFloorTest,
    "CombatCore.Attribute.HealthFloor",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCombatCoreHealthFloorTest::RunTest(const FString& /*Parameters*/)
{
    UCombatAttributeComponent* Attributes = MakeTestAttributes();

    const float ActualDamage = Attributes->ApplyDamage(250.f, nullptr);

    TestEqual(TEXT("Actual damage is clamped to current health"), ActualDamage, 100.f);
    TestEqual(TEXT("Health does not go below zero"), Attributes->GetHealth(), 0.f);
    TestTrue(TEXT("Dead after lethal damage"), Attributes->IsDead());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCombatCoreHealCapTest,
    "CombatCore.Attribute.HealCap",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCombatCoreHealCapTest::RunTest(const FString& /*Parameters*/)
{
    UCombatAttributeComponent* Attributes = MakeTestAttributes();

    Attributes->ApplyDamage(40.f, nullptr);
    const float ActualHeal = Attributes->Heal(1000.f);

    TestEqual(TEXT("Actual heal is capped at missing health"), ActualHeal, 40.f);
    TestEqual(TEXT("Health is capped at max"), Attributes->GetHealth(), 100.f);
    TestEqual(TEXT("Health never exceeds max"), Attributes->GetHealth(), Attributes->GetMaxHealth());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCombatCoreStaminaTest,
    "CombatCore.Attribute.Stamina",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCombatCoreStaminaTest::RunTest(const FString& /*Parameters*/)
{
    UCombatAttributeComponent* Attributes = MakeTestAttributes();

    TestTrue(TEXT("Consume 60 from 100"), Attributes->TryConsumeStamina(60.f));
    TestFalse(TEXT("50 is refused when only 40 remains"), Attributes->TryConsumeStamina(50.f));
    TestEqual(TEXT("A refused consume does not deduct"), Attributes->GetStamina(), 40.f);
    TestTrue(TEXT("Exactly 40 is allowed"), Attributes->TryConsumeStamina(40.f));
    TestFalse(TEXT("No stamina left"), Attributes->TryConsumeStamina(1.f));
    TestEqual(TEXT("Stamina stays at zero"), Attributes->GetStamina(), 0.f);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCombatCoreDeathOnceTest,
    "CombatCore.Attribute.DeathOnce",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCombatCoreDeathOnceTest::RunTest(const FString& /*Parameters*/)
{
    UCombatAttributeComponent* Attributes = MakeTestAttributes();
    UCombatAttributeTestListener* Listener = NewObject<UCombatAttributeTestListener>();

    Attributes->OnDeath.AddDynamic(Listener, &UCombatAttributeTestListener::HandleDeath);

    Attributes->ApplyDamage(100.f, nullptr);
    Attributes->ApplyDamage(100.f, nullptr);

    TestEqual(TEXT("OnDeath broadcasts exactly once"), Listener->DeathCount, 1);
    TestEqual(TEXT("Damage after death is ignored"), Attributes->ApplyDamage(10.f, nullptr), 0.f);
    TestEqual(TEXT("Death count is still one"), Listener->DeathCount, 1);

    Attributes->OnDeath.RemoveDynamic(Listener, &UCombatAttributeTestListener::HandleDeath);

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
