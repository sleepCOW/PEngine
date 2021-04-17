#include "Core/Public/ConfReader.h"
#include "rapidJSON/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>
#include <cstdio>

using namespace rapidjson;

constexpr size_t BufferSize = 65536; // 64 Kb

CConfReader::CConfReader()
{
	ConfigurationDoc = OpenJSON(CONFIGURATION_PATH, true);
}

CConfReader::~CConfReader()
{
	/** Close all opened handles */
	for (auto& File : OpenedFiles)
	{
		SaveJSON(File.Handle, File.Json);
	}
}

int CConfReader::GetInt(const String& ValueName)
{
	return GetIntFrom(ValueName, GetConfigurationDoc());
}

int CConfReader::GetIntFrom(const String& ValueName, rapidjson::Document& JsonDocument)
{
	int Result = JsonDocument[ValueName.data()].GetInt();

#ifdef DEBUG_JSON
	fprintf(stdout, "CConfReader::GetStringFrom, \"%s\" = \"%d\"", ValueName.data(), Result);
#endif

	return Result;
}

String CConfReader::GetString(const String& ValueName)
{
	return GetStringFrom(ValueName, GetConfigurationDoc());
}

String CConfReader::GetStringFrom(const String& ValueName, rapidjson::Document& JsonDocument)
{
	String Result = JsonDocument[ValueName.data()].GetString();

#ifdef DEBUG_JSON
	fprintf(stdout, "CConfReader::GetStringFrom, \"%s\" = \"%s\"", ValueName.data(), Result.data());
#endif

	return Result;
}

CConfReader::SFile* CConfReader::OpenJSON(const String& Path, bool bCreateIfNull /*= false*/)
{
	assert(Path.size() != 0);

	FILE* FileHandle = nullptr;
	fopen_s(&FileHandle, Path.data(), "r+");
	if (!FileHandle)
	{
		fprintf(stdout, "CConfReader::OpenJSON Couldn't open file %s for Read/Write access!", Path.data());

		if (bCreateIfNull)
		{
			return CreateJSON(Path);
		}
		return nullptr;
	}
	fprintf(stdout, "CConfReader::OpenJSON Opened file %s for Read/Write access!", Path.data());

	char ReadBuffer[BufferSize];
	FileReadStream Is(FileHandle, ReadBuffer, BufferSize);

	// #TODO sleepCOW: Add check whether JSON file is valid!
	Document JsonDoc;
	JsonDoc.ParseStream(Is);
	OpenedFiles.push_back({ FileHandle, std::move(JsonDoc) });

	// Return pointer to pushed element
	return &OpenedFiles.back();
}

CConfReader::SFile* CConfReader::CreateJSON(const String& Path)
{
	assert(Path.size() != 0);
	
	FILE* FileHandle = nullptr;
	fopen_s(&FileHandle, Path.data(), "w");

	if (!FileHandle)
	{
		fprintf(stdout, "CConfReader::CreateJSON Couldn't create file %s for Read/Write access!", Path.data());
		return nullptr;
	}
	fprintf(stdout, "CConfReader::CreateJSON Created file %s for Read/Write access!", Path.data());

	OpenedFiles.push_back({ FileHandle, Document() });
	return &OpenedFiles.back();
}

void CConfReader::SaveJSON(FILE* FileHandle, rapidjson::Document& JsonDocument)
{
	char WriteBuffer[BufferSize];
	FileWriteStream os(FileHandle, WriteBuffer, BufferSize);
	Writer<FileWriteStream> Writer(os);
	JsonDocument.Accept(Writer);
}
