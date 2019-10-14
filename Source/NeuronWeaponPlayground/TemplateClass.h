// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// NWP

// UE

// Generated
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TemplateClass.generated.h"

// Types

// Delegates

/**
 * Small description explaining the responsibilities of the class.
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API UTemplateClass : public UObject
{
	GENERATED_BODY()

// Friend class
friend class UObject;

// Types
public:

protected:

private:

// Constructors
public:

protected:

private:

// Events / Delegates
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

	// Small description explaining the functionality of the method ExampleMethodOne
	void ExampleMethodOne();

	// Small description explaining the functionality of the method ExampleMethodTwo
	void ExampleMethodTwo();

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
- If the comment is a line, // will be used. If you have more than one line, /** * / (without the space) will be used. If it is a type (class, struct...) description,
  the second indicated syntax will be used.
- If the comment is a line, no dot will be used unless it is a type (class, struct...) description. These last ones always end with a dot.

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
  For example, you can add the word "Current" to the member variable and nothing to the local variable: CurrentIndex (member variable) - Index (local variable).

# Event / Delegates:
#####################
- Basic concepts to avoid confusions:
	- Events: Do not confuse with event delegates. Member functions which have the FunctionSpecifier BlueprintNativeEvent or BlueprintImplementableEvent.
	- Delegates: Data types or variables that reference and execute member functions on C++ objects. For more information: https://docs.unrealengine.com/en-US/Programming/UnrealArchitecture/Delegates/index.html.
	- Event delegates: Do not confuse with events. A type of delegate which can be bound to multiple functions and execute them all at once only by the class that declares the event delegate.
	  For more information: https://docs.unrealengine.com/en-US/Programming/UnrealArchitecture/Delegates/Events/index.html.
- The "Delegates" section refers to the delegate type declaration and the "Events / Delegates" section refers to members (functions or variables) referring to events or delegates.
- Event declaration naming example:
	On{EventName}{CustomSuffix} (BlueprintNativeEvent or BlueprintImplementableEvent should appear obove the function declaration as a FunctionSpecifier)
- Delegate type declaration naming example:
	F{ProjectPrefix}On{DelegateName}{CustomSuffix}
- Delegate variable declaration naming example:
	On{DelegateName}{CustomSuffix}
- The "CustomSuffix" depends on whether it is an event or the type of delegate:
	- If it is an event => Event.
	- If it is a simple delegate => Delegate.
	- If it is a multicast delegate => MulticastDelegate.
	- If it is an event delegate => EventDelegate.
- Whether it is dynamic or not is not indicated in the name of the event/delegate.

# Miscellaneous:
###############
- Use Transient and SkipSerialization for cache variables. 

**/
};
