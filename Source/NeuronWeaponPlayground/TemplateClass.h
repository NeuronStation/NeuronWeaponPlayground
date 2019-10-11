// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// UE
// NWP

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TemplateClass.generated.h"

// Events / Delegates

/**
 * Small description explaining the responsibilities of the class
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API UTemplateClass : public UObject
{
	GENERATED_BODY()

// Friend class
friend class UObject;

// Constructors
public:

protected:

private:

// Member functions
public:

protected:

private:

// Member variables
public: 
// IMPORTANT!: Public variables can be allowed for structs in some cases (for example: Transfer Object pattern). Structs do not require to fulfill the encapsulation principle.
// The encapsulation principle is a characteristic of the OOP. Structs were used before the OOP appeared.

protected:

private:

/**

# Basic:
#############
- Curly braces should always appear even if it is for one line.
- Starting curly brace should appear on a new line.
- No need to write == nullptr when null checking.
- It is advisable to leave a blank separation line after having declared a set of local variables.
- Try to use early returns as much as possible.
- Should use the default visual studio configuration for tabs. This means that when Tab is pressed, a tab is inserted and not 4 spaces.
- A blank line should appear at the end of each .h or .cpp file.
- The specifier auto is allowed to be used when iterating on a collection (TArray, TMap, TSet...).
- Everything declared in the .h must have at least one line of description.

# Separators:
#############

	- Inside each access modifier, the code should be ordered by function responsibility. Use the functional separator shown below.

	///////////////////////////////////////////////////////////////////////////
	// Functional separator

	- In some cases, it is also interesting to separate code blocks. Otherwise it is a functional separation to use the separator shown below.

	// ------------------------------------- Non-functional separator -------------------------------------

# Overwritten methods example:
##############################

	/// Grandparent interface begin
	// Small description explaining the functionality of the method
	virtual void MyFirstOverridenMethod() override;
	/// Grandparent interface end

	/// Parent interface begin
	// Small description explaining the functionality of the method
	virtual void MySecondOverridenMethod() override;
	/// Parent interface end

	- Use virtual override always.
	- Ordered by kinship. The farthest relative above.

# Prefix:
#########
- Use b for booleans.
- Use _ for function signature variables.
- Use different words to distinguish local variables from member variables since m_ is shown in the editor if it is configurable from the editor.
  For example, you can add the word "Current" to the member variable and nothing to the local variable: CurrentIndex (member variable) - Index (local variable)

# Miscellaneous:
###############
- Use Transient and SkipSerialization for cache variables. 

**/
};
