#include "core.hpp"

int main()
{
	if (!FindWindowA(nullptr, "Roblox"))
	{
		printf("Roblox process is not active.\n");
		std::cin.get();
		return 0;
	}

	uintptr_t data_model = 0;

#pragma warning( push )
#pragma warning( disable : 26815)
	std::string log_path = std::format("C:\\Users\\{}\\AppData\\Local\\Roblox\\logs", 
		getenv("username"));
#pragma warning( pop )

	std::vector<std::filesystem::path> log_files = {};
	for (const auto& log_entry : std::filesystem::directory_iterator(log_path))
	{
		if (log_entry.is_regular_file() && log_entry.path().extension() == ".log" &&
			log_entry.path().filename().string().find("Player") != std::string::npos)
		{
			log_files.push_back(log_entry.path());
		}
	}

	if (log_files.empty())
	{
		printf("Failed to get logs.\n");
		std::cin.get();
		return 0;
	}

	std::sort(log_files.begin(), log_files.end(), [](const std::wstring& a, const std::wstring& b) {
		return std::filesystem::last_write_time(a) > std::filesystem::last_write_time(b);
	});

	auto latest_log = log_files[0];
	std::ifstream log_data(latest_log);

	std::string line = "";
	while (data_model == 0)
	{
		std::getline(log_data, line);
		if (line.contains("initialized DataModel("))
		{
			line = line.substr(line.find("initialized DataModel(") + 0x16);
			line = line.substr(0, line.find(')'));
			data_model = std::strtoull(line.c_str(), nullptr, 0x10);
		}
	}

#pragma warning( push )
#pragma warning( disable : 6328)
	printf("DataModel: 0x%08x\n", data_model);
#pragma warning( pop )
	std::cin.get();
	return 0;
}