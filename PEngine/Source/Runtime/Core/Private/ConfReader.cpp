#include "Core/Public/ConfReader.h"
#include "rapidJSON/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>
#include <cstdio>

using namespace rapidjson;

constexpr size_t BufferSize = 65536; // 64 Kb

CFileManager::CFileManager()
{
	ConfigurationDoc = OpenJSON(CONFIGURATION_PATH, true);

	rapidjson::Document& Config = ConfigurationDoc->Json;
	if (Config.HasParseError() || !Config.IsObject())
	{
		Config.SetObject();
		Config.AddMember("StartupLevel", Value(""), Config.GetAllocator());
	}
}

CFileManager::~CFileManager()
{
	/** Close all opened handles */
	for (auto& File : OpenedFiles)
	{
		SaveJSON(File.Handle, File.FileName, File.Json);
	}
}

int CFileManager::GetInt(const String& ValueName)
{
	return GetIntFrom(ValueName, GetConfigurationDoc());
}

int CFileManager::GetIntFrom(const String& ValueName, rapidjson::Document& JsonDocument)
{
	int Result = JsonDocument[ValueName.data()].GetInt();

#ifdef DEBUG_JSON
	fprintf(stdout, "CConfReader::GetStringFrom, \"%s\" = \"%d\"\n", ValueName.data(), Result);
#endif

	return Result;
}

String CFileManager::GetString(const String& ValueName)
{
	return GetStringFrom(ValueName, GetConfigurationDoc());
}

String CFileManager::GetStringFrom(const String& ValueName, rapidjson::Document& JsonDocument)
{
	String Result = JsonDocument[ValueName.data()].GetString();

#ifdef DEBUG_JSON
	fprintf(stdout, "CConfReader::GetStringFrom, \"%s\" = \"%s\"\n", ValueName.data(), Result.data());
#endif

	return Result;
}

CFileManager::SFile* CFileManager::OpenJSON(const String& Path, bool bCreateIfNull /*= false*/)
{
	// #TODO sleepCOW: Add check whether such file was already opened!
	assert(Path.size() != 0);

	FILE* FileHandle = nullptr;
	fopen_s(&FileHandle, Path.data(), "r+");
	if (!FileHandle)
	{
		fprintf(stdout, "CConfReader::OpenJSON Couldn't open file %s for Read/Write access!\n", Path.data());

		if (bCreateIfNull)
		{
			return CreateJSON(Path);
		}
		return nullptr;
	}
	fprintf(stdout, "CConfReader::OpenJSON Opened file %s for Read/Write access!\n", Path.data());

	char ReadBuffer[BufferSize];
	FileReadStream Is(FileHandle, ReadBuffer, BufferSize);

	// #TODO sleepCOW: Add check whether JSON file is valid!
	Document JsonDoc;
	JsonDoc.ParseStream(Is);
	OpenedFiles.push_back({ FileHandle, Path, std::move(JsonDoc) });

	// Return pointer to pushed element
	return &OpenedFiles.back();
}

CFileManager::SFile* CFileManager::CreateJSON(const String& Path)
{
	assert(Path.size() != 0);
	
	FILE* FileHandle = nullptr;
	fopen_s(&FileHandle, Path.data(), "w");

	if (!FileHandle)
	{
		fprintf(stdout, "CConfReader::CreateJSON Couldn't create file %s for Read/Write access!\n", Path.data());
		return nullptr;
	}
	fprintf(stdout, "CConfReader::CreateJSON Created file %s for Read/Write access!\n", Path.data());

	OpenedFiles.push_back({ FileHandle, Path, Document() });
	return &OpenedFiles.back();
}

void CFileManager::SaveJSON(const String& Path, rapidjson::Document& JsonDocument)
{
	SFile* File = OpenJSON(Path, true);
	File->Json = std::move(JsonDocument);
	SaveJSON(File->Handle, Path, JsonDocument);
}

void CFileManager::SaveJSON(FILE*& FileHandle, const String& Path, rapidjson::Document& JsonDocument)
{
	// Reopen to erase content
	freopen_s(&FileHandle, Path.data(), "w", FileHandle);

	char WriteBuffer[BufferSize];
	FileWriteStream os(FileHandle, WriteBuffer, BufferSize);
	Writer<FileWriteStream> Writer(os);
	JsonDocument.Accept(Writer);
}
