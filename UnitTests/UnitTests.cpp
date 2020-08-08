#include "pch.h"
#include "CppUnitTest.h"

#include "../ECS/src/ECS.hpp"
#include <iostream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{

	public:
		
		// Components
		struct TestComponent
		{
			int val;

			TestComponent() = default;
			TestComponent(int val)
				: val(val)
			{
			}

			bool operator==(TestComponent& t)
			{
				return this->val == t.val;
			}
		};

		// Systems
		class TestSystem : public System
		{
		public:
			int val;

			TestSystem(int val)
				: val(val)
			{
			}

			void Update(ECS& ecs)
			{
				for (const auto& entity : m_Entities)
				{
					auto& testComponent = ecs.GetComponent<TestComponent>(entity);
					testComponent.val -= 1;
				}
			}
		};


		TEST_METHOD(TestInitialization)
		{
			ECS ecs;
			ecs.Init();
		}

		TEST_METHOD(TestComponentRegistering)
		{
			ECS ecs;
			ecs.Init();

			ecs.RegisterComponent<TestComponent>();
		}
		TEST_METHOD(TestSystemRegistering)
		{
			ECS ecs;
			ecs.Init();

			auto system = ecs.RegisterSystem<TestSystem>(10);
			Assert::IsTrue(system != nullptr);
		}

		TEST_METHOD(TestSystemSignatureAssignment)
		{
			ECS ecs;
			ecs.Init();

			ecs.RegisterComponent<TestComponent>();
			ecs.RegisterSystem<TestSystem>(10);
			
			Signature signature;
			signature.set(ecs.GetComponentType<TestComponent>());

			ecs.SetSystemSignature<TestSystem>(signature);

			Signature targetSignature;
			targetSignature.set(ecs.GetComponentType<TestComponent>(), true);

			Assert::IsTrue(targetSignature == ecs.GetSystemManager()->GetSignature<TestSystem>());
		}

		TEST_METHOD(TestCreateEntity)
		{
			ECS ecs;
			ecs.Init();

			Entity entity = ecs.CreateEntity();
			Entity entity2 = ecs.CreateEntity();

			Assert::IsTrue(entity == 0);
			Assert::IsTrue(entity2 == 1);
		}
		
		TEST_METHOD(TestAddComponent)
		{
			ECS ecs;
			ecs.Init();

			ecs.RegisterComponent<TestComponent>();

			Entity entity = ecs.CreateEntity();
			ecs.AddComponent(entity, TestComponent(10));

			TestComponent target = TestComponent(10);

			Assert::IsTrue(ecs.GetComponent<TestComponent>(entity) == target);
		}

		TEST_METHOD(TestSystemWorking)
		{
			ECS ecs;
			ecs.Init();

			ecs.RegisterComponent<TestComponent>();
			auto system = ecs.RegisterSystem<TestSystem>(10);

			Signature signature;
			signature.set(ecs.GetComponentType<TestComponent>(), true);
			ecs.SetSystemSignature<TestSystem>(signature);

			Entity entity = ecs.CreateEntity();
			ecs.AddComponent(entity, TestComponent(10));

			Assert::IsTrue(system->m_Entities.size() == 1);

			system->Update(ecs);
			Assert::IsTrue(ecs.GetComponent<TestComponent>(entity).val == 9);
		}

	};
}
