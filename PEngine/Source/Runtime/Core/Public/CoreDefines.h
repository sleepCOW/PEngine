#pragma once

#include <type_traits>

#define TO_STR(ClassMember) #ClassMember

#define DECLARE_CLASS( ClassName, BaseClassName ) \
	private: \
		using Super = BaseClassName; \
		using ThisClass = ClassName; \
	public: \
		virtual const char* GetClassName() const { return #ClassName; }; \
	DECLARE_META()

#define DECLARE_META() \
	public: \
		static CReflectionData ReflectionData; \
	private:

// #TODO sleepCOW: avoid the need of including Level.h for DEFINE_META to compile!
#define DEFINE_META(ClassName) \
	CReflectionData ClassName::ReflectionData = CReflectionData(#ClassName, std::is_base_of<CComponent, ClassName>::value, &NewObject<ClassName>, &SpawnActor<ClassName>);