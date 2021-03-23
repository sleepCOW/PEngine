#pragma once

#define DECLARE_CLASS( ClassName, BaseClassName ) \
	private: \
		using Super = BaseClassName; \
		using ThisClass = ClassName;