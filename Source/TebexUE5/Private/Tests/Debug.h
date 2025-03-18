#pragma once

template <typename T>
void PrintUStructProperties(const T& Ref)
{
	// Get the struct type for Ref type and iterate over its properties
	if (const UStruct* StructType = T::StaticStruct()) {
		// Iterates over all properties in the struct, convert the value to a string, and output the name and value to log
		for (TFieldIterator<FProperty> PropertyIt(StructType); PropertyIt; ++PropertyIt) {
			const FProperty* Property = *PropertyIt;
			FString PropertyName = Property->GetName();

			const void* PropertyValue = Property->ContainerPtrToValuePtr<void>(&Ref);
			FString ValueAsString;
			Property->ExportTextItem_Direct(ValueAsString, PropertyValue, nullptr, nullptr, PPF_None);

			UE_LOG(LogTebex, Display, TEXT("%s = %s"), *PropertyName, *ValueAsString);
		}
	} else { // Not a UStruct
		UE_LOG(LogTebex, Error, TEXT("Failed to process provided Ref as a UStruct."));
	}
}