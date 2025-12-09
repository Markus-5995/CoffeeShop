#pragma once
#include <string_view>
#include <vector>
#include <memory>

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#define MY_ENCODING "ISO-8859-1"
#include <iostream>
#include <ranges>

namespace CoffeeShop
{
using XmlNodeHandle = uint32_t;

struct Node
{
    std::string name;
    std::vector<std::pair<std::string, std::string>> att;
};

class XmlWriter
{
public:
    explicit XmlWriter(std::string_view filename)
    {
        writer = xmlNewTextWriterFilename(filename.data(), 0);
        if (writer == NULL) {
            throw std::runtime_error("XmlWriter: Error creating the xml writer");
        }

        int rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
        if (rc < 0) {
            throw std::runtime_error("XmlWriter: Error at xmlTextWriterStartDocument");
        }
    }
    ~XmlWriter()
    {
        int rc = xmlTextWriterEndDocument(writer);
        if (rc < 0) {
            std::cerr << "XmlWriter: Error at xmlTextWriterEndDocument" << std::endl;
            return;
        }

        xmlFreeTextWriter(writer);
    }
    XmlNodeHandle operator()(const Node& nodeInfo)
    {
        if (nodeStack.size() > 0)
        {
            throw std::runtime_error("XmlWriter: Root node already created!");
        }
        return create(nodeInfo);
    }

    XmlNodeHandle operator()(XmlNodeHandle parentNode, const Node& nodeInfo)
    {
        if (nodeStack.size() == 0)
        {
            throw std::runtime_error("XmlWriter: Create root node already first!");
        }

        if (nodeStack.back() == parentNode)
        {
            return create(nodeInfo);
        }

        auto view = nodeStack
                    | std::ranges::views::drop_while([parentNode](const auto & node) { return node != parentNode;});

        size_t clearCount = view.size() - 1;
        if (clearCount == 0)
        {
            throw std::runtime_error("XmlWriter: Provided parent is not valid!");
        }
        nodeStack.erase(nodeStack.end() - clearCount, nodeStack.end());

        for (size_t i = 0; i < clearCount; i++)
        {
            int rc = xmlTextWriterEndElement(writer);
            if (rc < 0) {
                throw std::runtime_error("XmlWriter: Error at xmlTextWriterEndElement");
            }
        }
        return create(nodeInfo);
    }

private:
    XmlNodeHandle create(const Node& nodeInfo)
    {
        int rc = xmlTextWriterStartElement(writer, BAD_CAST nodeInfo.name.c_str());
        if (rc < 0) {
            throw std::runtime_error("XmlWriter: Error at xmlTextWriterStartElement");
        }

        for (const auto& pair: nodeInfo.att)
        {
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST pair.first.c_str(),
                                             BAD_CAST pair.second.c_str());
            if (rc < 0) {
                throw std::runtime_error("XmlWriter: Error at xmlTextWriterWriteAttribute");
            }
        }

        auto handle = nextHandle++;
        nodeStack.push_back(handle);
        return handle;
    }
    std::vector<XmlNodeHandle> nodeStack;

    xmlTextWriterPtr writer;
    XmlNodeHandle nextHandle {1};

};


}
