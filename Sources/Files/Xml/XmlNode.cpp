#include "XmlNode.hpp"

namespace acid
{
	XmlNode::XmlNode(XmlNode *parent, const std::string &attributes, const std::string &content) :
		m_parent(parent),
		m_children(std::vector<XmlNode *>()),
		m_attributes(attributes),
		m_content(content)
	{
	}

	XmlNode::~XmlNode()
	{
		for (auto &child : m_children)
		{
			delete child;
		}
	}

	void XmlNode::AppendData(const std::shared_ptr<Metadata> &source, std::stringstream &builder, const int32_t &indentation)
	{
		std::stringstream indents;

		for (int32_t i = 0; i < indentation; i++)
		{
			indents << "\t";
		}

		std::stringstream nameAttributes;
		nameAttributes << source->GetName();

		for (auto &attribute : source->GetAttributes())
		{
			nameAttributes << " " << attribute.first << "=\"" << attribute.second << "\"";
		}

		std::string nameAndAttribs = String::Trim(nameAttributes.str());

		builder << indents.str();

		if (source->GetName()[0] == '?')
		{
			builder << "<" << nameAndAttribs << "?>\n";

			for (auto &child : source->GetChildren())
			{
				AppendData(child, builder, indentation);
			}

			return;
		}

		if (source->GetChildren().empty() && source->GetValue().empty())
		{
			builder << "<" << nameAndAttribs << "/>\n";
			return;
		}

		builder << "<" << nameAndAttribs << ">" << source->GetValue();

		if (!source->GetChildren().empty())
		{
			builder << "\n";

			for (auto &child : source->GetChildren())
			{
				AppendData(child, builder, indentation + 1);
			}

			builder << indents.str();
		}

		builder << "</" << source->GetName() << ">\n";
	}

	std::shared_ptr<Metadata> &XmlNode::Convert(const XmlNode &source, std::shared_ptr<Metadata> &parent, const bool &isTopSection)
	{
		auto thisValue = parent;

		int32_t firstSpace = String::FindCharPos(source.m_attributes, ' ');
		std::string name = String::Substring(source.m_attributes, 0, firstSpace);
		name = String::Trim(name);
		std::string attributes = String::Substring(source.m_attributes, firstSpace + 1, source.m_attributes.size());
		attributes = String::Trim(attributes);

		if (attributes[attributes.size() - 1] == '/' || attributes[attributes.size() - 1] == '?')
		{
			attributes.pop_back();
		}

		attributes = String::Trim(attributes);

		std::map<std::string, std::string> parseAttributes = {};

		if (!attributes.empty())
		{
			std::string currentKey = "";
			std::string summation = "";

			for (char &c : attributes)
			{
				switch (c)
				{
				case '"':
				{
					if (currentKey.empty())
					{
						currentKey = summation;
						summation.clear();
						continue;
					}

					parseAttributes.emplace(String::Trim(currentKey), String::Trim(summation));
					currentKey.clear();
					summation.clear();
					break;
				}
				case '=':
				{
					if (!currentKey.empty())
					{
						summation += c;
					}

					break;
				}
				default:
				{
					summation += c;
					break;
				}
				}
			}
		}

		if (!isTopSection)
		{
			thisValue = std::make_shared<Metadata>(name, source.m_content, parseAttributes);
			parent->AddChild(thisValue);
		}
		else
		{
			parent->SetName(name);
			parent->SetValue(source.m_content);
			parent->SetAttributes(parseAttributes);
		}

		for (auto &child : source.m_children)
		{
			Convert(*child, thisValue, false);
		}

		return thisValue;
	}
}
