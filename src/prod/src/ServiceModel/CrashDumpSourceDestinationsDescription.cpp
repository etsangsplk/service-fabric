// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "stdafx.h"

using namespace std;
using namespace Common;
using namespace ServiceModel;

CrashDumpSourceDestinationsDescription::CrashDumpSourceDestinationsDescription()
    : LocalStore(),
    FileStore(),
    AzureBlob()
{
}

CrashDumpSourceDestinationsDescription::CrashDumpSourceDestinationsDescription(
    CrashDumpSourceDestinationsDescription const & other)
    : LocalStore(other.LocalStore),
    FileStore(other.FileStore),
    AzureBlob(other.AzureBlob)
{
}

CrashDumpSourceDestinationsDescription::CrashDumpSourceDestinationsDescription(CrashDumpSourceDestinationsDescription && other)
    : LocalStore(move(other.LocalStore)),
    FileStore(move(other.FileStore)),
    AzureBlob(move(other.AzureBlob))
{
}

CrashDumpSourceDestinationsDescription const & CrashDumpSourceDestinationsDescription::operator = (CrashDumpSourceDestinationsDescription const & other)
{
    if (this != &other)
    {
        this->LocalStore = other.LocalStore;
        this->FileStore = other.FileStore;
        this->AzureBlob = other.AzureBlob;
    }
    return *this;
}

CrashDumpSourceDestinationsDescription const & CrashDumpSourceDestinationsDescription::operator = (CrashDumpSourceDestinationsDescription && other)
{
    if (this != &other)
    {
        this->LocalStore = move(other.LocalStore);
        this->FileStore = move(other.FileStore);
        this->AzureBlob = move(other.AzureBlob);
    }
    return *this;
}

bool CrashDumpSourceDestinationsDescription::operator == (CrashDumpSourceDestinationsDescription const & other) const
{
    bool equals = true;

    equals = this->LocalStore.size() == other.LocalStore.size();
    if (!equals) { return equals; }

    for (size_t ix = 0; ix < this->LocalStore.size(); ++ix)
    {
        equals = this->LocalStore[ix] == other.LocalStore[ix];
        if (!equals) { return equals; }
    }
    
    equals = this->FileStore.size() == other.FileStore.size();
    if (!equals) { return equals; }

    for (size_t ix = 0; ix < this->FileStore.size(); ++ix)
    {
        equals = this->FileStore[ix] == other.FileStore[ix];
        if (!equals) { return equals; }
    }
    
    equals = this->AzureBlob.size() == other.AzureBlob.size();
    if (!equals) { return equals; }

    for (size_t ix = 0; ix < this->AzureBlob.size(); ++ix)
    {
        equals = this->AzureBlob[ix] == other.AzureBlob[ix];
        if (!equals) { return equals; }
    }

    return equals;
}

bool CrashDumpSourceDestinationsDescription::operator != (CrashDumpSourceDestinationsDescription const & other) const
{
    return !(*this == other);
}

void CrashDumpSourceDestinationsDescription::WriteTo(TextWriter & w, FormatOptions const &) const
{
    w.Write("CrashDumpSourceDestinationsDescription { ");

    w.Write("LocalStore = {");
    w.Write(this->LocalStore);
    w.Write("}, ");

    w.Write("FileStore = {");
    w.Write(this->FileStore);
    w.Write("}, ");

    w.Write("AzureBlob = {");
    w.Write(this->AzureBlob);
    w.Write("}, ");

    w.Write("}");
}

void CrashDumpSourceDestinationsDescription::ReadFromXml(
    XmlReaderUPtr const & xmlReader)
{
    clear();

    xmlReader->StartElement(
        *SchemaNames::Element_Destinations,
        *SchemaNames::Namespace,
        false);

    if (xmlReader->IsEmptyElement())
    {
        // <Destinations />
        xmlReader->ReadElement();
        return;
    }

    xmlReader->ReadStartElement();

    // <LocalStore>
    bool localStoresAvailable = true;
    while (localStoresAvailable)
    {
        if (xmlReader->IsStartElement(
            *SchemaNames::Element_LocalStore,
            *SchemaNames::Namespace))
        {
            LocalStoreDescription localStore;
            localStore.ReadFromXml(xmlReader);
            this->LocalStore.push_back(move(localStore));
        }
        else
        {
            localStoresAvailable = false;
        }
    }

    // <FileStore>
    bool fileStoresAvailable = true;
    while (fileStoresAvailable)
    {
        if (xmlReader->IsStartElement(
            *SchemaNames::Element_FileStore,
            *SchemaNames::Namespace))
        {
            FileStoreDescription fileStore;
            fileStore.ReadFromXml(xmlReader);
            this->FileStore.push_back(move(fileStore));
        }
        else
        {
            fileStoresAvailable = false;
        }
    }

    // <AzureBlob>
    bool azureBlobsAvailable = true;
    while (azureBlobsAvailable)
    {
        if (xmlReader->IsStartElement(
            *SchemaNames::Element_AzureBlob,
            *SchemaNames::Namespace))
        {
            AzureBlobDescription azureBlob;
            azureBlob.ReadFromXml(xmlReader);
            this->AzureBlob.push_back(move(azureBlob));
        }
        else
        {
            azureBlobsAvailable = false;
        }
    }

    // </Destinations>
    xmlReader->ReadEndElement();
}


ErrorCode CrashDumpSourceDestinationsDescription::WriteToXml(XmlWriterUPtr const & xmlWriter)
{	//<Destinations>
	ErrorCode er = xmlWriter->WriteStartElement(*SchemaNames::Element_Destinations, L"", *SchemaNames::Namespace);
	if (!er.IsSuccess())
	{
		return er;
	}
	for (auto i = 0; i < LocalStore.size(); i++)
	{
		er = LocalStore[i].WriteToXml(xmlWriter);
		if (!er.IsSuccess())
		{
			return er;
		}
	}

	for (auto i = 0; i < FileStore.size(); i++)
	{
		er = FileStore[i].WriteToXml(xmlWriter);
		if (!er.IsSuccess())
		{
			return er;
		}
	}

	for (auto i = 0; i < AzureBlob.size(); i++)
	{
		er = AzureBlob[i].WriteToXml(xmlWriter);
		if (!er.IsSuccess())
		{
			return er;
		}
	}


	//</Destinations>
	return xmlWriter->WriteEndElement();
}

void CrashDumpSourceDestinationsDescription::clear()
{
    this->LocalStore.clear();
    this->FileStore.clear();
    this->AzureBlob.clear();
}
bool CrashDumpSourceDestinationsDescription::empty()
{
	return LocalStore.empty() &&
		FileStore.empty() &&
		AzureBlob.empty();
}
