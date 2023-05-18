#pragma once
#include "KeyType.h"
#include "KeyData.generated.h"

USTRUCT(BlueprintType)
struct FBsKeyData
{
	GENERATED_BODY()
	
	/**
	 * @brief Name of the Key.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName KeyName;

	/**
	 * @brief Description of the Key.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString KeyDescription;

	/**
	 * @brief Identifier of the Key. To be used along with KeyType.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString KeyIdentifier;

	/**
	 * @brief Type of the Key.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBsKeyType KeyType;

	/**
	 * @brief Copies the KeyData to this Key.
	 * @param KeyData Key data to copy from.
	 */
	void CopyKeyData(FBsKeyData KeyData)
	{
		KeyName = KeyData.KeyName;
		KeyDescription = KeyData.KeyDescription;
		KeyIdentifier = KeyData.KeyIdentifier;
		KeyType = KeyData.KeyType;
	}
};
