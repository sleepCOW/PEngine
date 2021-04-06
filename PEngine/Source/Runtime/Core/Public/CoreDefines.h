#pragma once

#ifdef WITH_EDITOR
#define DECLARE_CLASS( ClassName, BaseClassName ) \
	private: \
		using Super = BaseClassName; \
		using ThisClass = ClassName; \
	DECLARE_META()	
#else
#define DECLARE_CLASS( ClassName, BaseClassName ) \
	private: \
		using Super = BaseClassName; \
		using ThisClass = ClassName;
#endif

#ifdef WITH_EDITOR
#define DECLARE_META() \
	public: \
		static CReflectionData ReflectionData; \
	private:
#else
#define DECLARE_META()
#endif

#ifdef WITH_EDITOR
#define DEFINE_META(ClassName) \
	CReflectionData ClassName::ReflectionData = CReflectionData(#ClassName);
#else
#define DEFINE_META(ClassName)
#endif