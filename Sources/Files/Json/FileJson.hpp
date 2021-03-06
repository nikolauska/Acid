#pragma once

#include <string>
#include <utility>
#include <vector>
#include "Files/IFile.hpp"
#include "Helpers/String.hpp"
#include "JsonSection.hpp"

namespace acid
{
	class ACID_EXPORT FileJson :
		public IFile
	{
	private:
		std::string m_filename;
		std::shared_ptr<Metadata> m_parent;
	public:
		FileJson(const std::string &filename);

		~FileJson();

		void Load() override;

		void Save() override;

		void Clear() override;

		std::string GetFilename() const override { return m_filename; }

		void SetFilename(const std::string &filename) override { m_filename = filename; }

		std::shared_ptr<Metadata> GetParent() const override { return m_parent; }

		std::shared_ptr<Metadata> GetChild(const std::string &name) const { return m_parent->FindChild(name); }
	private:
		void Verify();
	};
}
