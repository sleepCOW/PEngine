#pragma once

#include "CoreMinimal.h"
#include "rapidJSON/document.h"

class CConfReader
{
public:
	struct SFile
	{
		FILE* Handle = nullptr;
		rapidjson::Document Json;
	};
	
	CConfReader();
	~CConfReader();

	/** Get int value from configuration document */
	int GetInt(const String& ValueName);

	/** Get int value from specified json document */
	int GetIntFrom(const String& ValueName, rapidjson::Document& JsonDocument);

	/** Get string value from configuration document */
	String GetString(const String& ValueName);

	/** Get string value from specified json document */
	String GetStringFrom(const String& ValueName, rapidjson::Document& JsonDocument);

	rapidjson::Document& GetConfigurationDoc() { return ConfigurationDoc->Json; }

	/**
	 * Open JSON file with Read/Write access
	 * @param Path path to the file
	 * @param bCreateIfNull if file doesn't exist create it
	 * @return pointer to opened/created file from OpenedFiles vector or null on failure
	 */
	SFile* OpenJSON(const String& Path, bool bCreateIfNull = false);

	/**
	 * Create Json file at the specified location
	 * @param Path path to the file
	 * @param bSuccess Whether function call was successful
	 * @param InitialData feel with data 
	 * @return opened or created (if bCreateIfNull = true) json document
	 */
	SFile* CreateJSON(const String& Path);

	/**
	 * Save json document to the provided path
	 */
	void SaveJSON(const String& Path, rapidjson::Document& JsonDocument);

	/**
	 * Save given json to provided file handle
	 */
	void SaveJSON(FILE* FileHandle, rapidjson::Document& JsonDocument);

protected:
	/** Ptr to ConfigurationDoc in OpenedFiles vector */
	SFile* ConfigurationDoc;

	Vector<SFile> OpenedFiles;

	Map<void*, int> PointerToId;
};