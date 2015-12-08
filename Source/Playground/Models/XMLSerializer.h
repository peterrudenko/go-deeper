/*
  ==============================================================================

    XMLSerializer.h
    Created: 17 Sep 2015 11:51:04am
    Author:  Peter Rudenko

  ==============================================================================
*/

#ifndef XMLSERIALIZER_H_INCLUDED
#define XMLSERIALIZER_H_INCLUDED

// The JUCE-based serializer example

class XMLSerializer final
{
private:
    
    class Context : public TinyRNN::SerializationContext
    {
    public:
        
        using Ptr = std::shared_ptr<Context>;
        
        Context(XmlElement *rootElement);
        
        virtual void setRealProperty(double value, const std::string &key) override;
        virtual double getRealProperty(const std::string &key) const override;
        virtual void setNumberProperty(long long value, const std::string &key) override;
        virtual long long getNumberProperty(const std::string &key) const override;
        virtual void setStringProperty(const std::string &value, const std::string &key) override;
        virtual std::string getStringProperty(const std::string &key) const override;
        virtual size_t getNumChildrenContexts() const override;
        virtual TinyRNN::SerializationContext::Ptr getChildContext(int index) const override;
        virtual TinyRNN::SerializationContext::Ptr getChildContext(const std::string &key) const override;
        virtual TinyRNN::SerializationContext::Ptr createChildContext(const std::string &key) override;
        
    private:
        
        XmlElement *root;
    };
    
public:
    
    XMLSerializer() {}
    
    String serialize(TinyRNN::SerializedObject::Ptr target, const std::string &rootNodeName) const;
    void deserialize(TinyRNN::SerializedObject::Ptr target, const std::string &data);
    
    template<typename T>
    T *createAndLoadInstanceOfType(const File file)
    {
        if (file.existsAsFile())
        {
            T *instance = new T();
            this->deserialize(file.loadFileAsString(), *instance);
            return instance;
        }
        
        return nullptr;
    }
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XMLSerializer)
    
};

#endif  // XMLSERIALIZER_H_INCLUDED