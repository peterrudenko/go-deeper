/*
  ==============================================================================

    TextTrainIteration.cpp
    Created: 8 Dec 2015 5:45:15pm
    Author:  Peter Rudenko

  ==============================================================================
*/

#include "Precompiled.h"
#include "TextTrainIteration.h"

#if defined TRAINING_MODE
#include "GoDeeper.h"
#endif

#define ALPHABET_RANGE 64

#if defined TRAINING_MODE
TextTrainIteration::TextTrainIteration() {}
#else
TextTrainIteration::TextTrainIteration(TinyRNN::HardcodedNetwork::Ptr targetNetwork) :
clNetwork(targetNetwork)
{
    jassert(targetNetwork->getContext()->getOutputs().size() == ALPHABET_RANGE);
}
#endif

#pragma mark - ASCII bastardization

int inputNodeIndexByChar192(juce_wchar character)
{
    int result = character;
    
    if (character >= 1040 &&
        character <= 1104) {
        result = 128 + (character - 1040);
    }

    return jmin(result, 192);
}

juce_wchar charByOutputNodeIndex192(int nodeIndex)
{
    juce_wchar result = nodeIndex;
    
    if (nodeIndex >= 128) {
        result = (nodeIndex - 128) + 1040;
    }
    
    return result;
}

static const int kCRReplacement = 36; // let's replace $ sign as the most useless on our case
static const int kBaseAnchor = -32;
static const int kCyrillicAnchor = 32;

int inputNodeIndexByChar64(juce_wchar character)
{
    int result = character + kBaseAnchor;
    
    // All cyrillic UTF-8 symbols: 1040 to 1104 (0410h - 0450h) (64 in total)
    // 0430 - 044F : lowercase cyrillic (32 in total)
    // 1072 - 1103
    
    // handle cyrillic symbols (ignoring case)
    if (character >= 1040 &&
        character <= 1071) {
        result = kCyrillicAnchor + (character - 1040);
    }
    
    if (character >= 1072 &&
        character <= 1103) {
        result = kCyrillicAnchor + (character - 1072);
    }
    
    // handle cr lf
    if (character == 10 ||
        character == 13) {
        result = kCRReplacement + kBaseAnchor;
    }
    
    return jmin(result, 64);
}

juce_wchar charByOutputNodeIndex64(int nodeIndex)
{
    juce_wchar result = nodeIndex - kBaseAnchor;
    
    // handle cyrillic symbols
    if (nodeIndex >= kCyrillicAnchor) {
        result = (nodeIndex + 1072 - kCyrillicAnchor);
    }
    
    // handle cr lf
    if (result == kCRReplacement) {
        result = 13;
    }
    
    return result;
}

#pragma mark - Processing

// Process one iteration of training.
void TextTrainIteration::processWith(const String &text)
{
    // 1. go through events and train the network
    
    int currentCharIndex = 0;
    
    // presuming that we have a lstm like
    // ALPHABET_RANGE -> ... -> ALPHABET_RANGE
    
    TinyRNN::HardcodedTrainingContext::RawData inputs;
    inputs.resize(ALPHABET_RANGE);
    
    TinyRNN::HardcodedTrainingContext::RawData targets;
    targets.resize(ALPHABET_RANGE);
    
    while (currentCharIndex < text.length())
    {
        // inputs and outputs are the float vectors of size ALPHABET_RANGE
        // inputs define the current character
        // outputs define the one to come next
        
        inputs.clear();
        int currentCharNodeIndex = inputNodeIndexByChar64(text[currentCharIndex]);
        for (int i = 0; i < ALPHABET_RANGE; ++i)
        {
            inputs[i] = (i == currentCharNodeIndex) ? 1.f : 0.f;
        }
        
        // now fix the targets
        targets.clear();
        const bool isLastChar = (currentCharIndex == (text.length() - 1));
        int nextCharNodeIndex = isLastChar ? inputNodeIndexByChar64('\n') : inputNodeIndexByChar64(currentCharIndex + 1);
        for (int i = 0; i < ALPHABET_RANGE; ++i)
        {
            targets[i] = (i == nextCharNodeIndex) ? 1.f : 0.f;
        }
        
        // train
#if defined TRAINING_MODE
        GoDeeperFeed(inputs.data());
        GoDeeperTrain(0.5f, targets.data());
#else
        this->clNetwork->feed(inputs);
        this->clNetwork->train(0.5, targets);
#endif
        
        currentCharIndex++;
    }
    
    // train with some empty passes
    static const size_t emptyTrainIterations = 50;
    std::fill(inputs.begin(), inputs.end(), 0.f);
    std::fill(targets.begin(), targets.end(), 0.f);
    
    for (size_t i = 0; i < emptyTrainIterations; ++i) {
#if defined TRAINING_MODE
        GoDeeperFeed(inputs.data());
        GoDeeperTrain(0.5f, targets.data());
#else
        this->clNetwork->feed(inputs);
        this->clNetwork->train(0.5, targets);
#endif
    }
}

String TextTrainIteration::generateSample() const
{
#if defined TRAINING_MODE
    String result;
    const int sampleLength = 2000;
    for (int i = 0; i < sampleLength; ++i)
    {
        GoDeeperFeed(kOutputs);
        
        int charIndex = 0;
        float maxProbability = -FLT_MAX;
        for (int j = 0; j < kOutputsSize; ++j)
        {
            if (maxProbability < kOutputs[j])
            {
                maxProbability = kOutputs[j];
                charIndex = j;
            }
        }
        
        result += charByOutputNodeIndex64(charIndex);
    }
    
    return result;
#else
    return String::empty;
#endif
}