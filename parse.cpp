#include "asset.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

std::map<std::string, Asset> parseCSV(const std::string &filename)
{
    std::map<std::string, Asset> assets;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Read header line
    std::getline(file, line);
    std::istringstream headerStream(line);
    std::string columnName;
    std::vector<std::string> assetNames;

    // Skip tix index as instruded
    std::getline(headerStream, columnName, ',');
    while (std::getline(headerStream, columnName, ','))
    {
        assets[columnName] = Asset(columnName);
        assetNames.push_back(columnName);
    }

    std::map<std::string, double> lastValidPrices;

    // Read data lines
    std::string cell;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);

        // Skip time index
        std::getline(lineStream, cell, ',');
        size_t i = 0;

        while (std::getline(lineStream, cell, ','))
        {
            double price;
            try
            {
                // Attempt to convert the cell to a double
                price = std::stod(cell);
                lastValidPrices[assetNames[i]] = price;
            }
            catch (const std::invalid_argument &)
            {
                // Check if there is a last valid price for this asset
                if (lastValidPrices.find(assetNames[i]) == lastValidPrices.end())
                {
                    std::cerr << "Error: No valid initial price for asset " << assetNames[i] << std::endl;
                    exit(EXIT_FAILURE);
                }
                // Use the last valid price
                price = lastValidPrices[assetNames[i]];
            }

            // Check if the asset already exists in the map
            auto it = assets.find(assetNames[i]);
            if (it == assets.end())
            {
                // If not found, create a new Asset and insert it into the map
                Asset newAsset(assetNames[i]);
                newAsset.addPrice(price);
                assets.insert(std::make_pair(assetNames[i], newAsset));
            }
            else
            {
                // If found, add the price to the existing Asset
                it->second.addPrice(price);
            }

            i++;
        }

        // Check for missing records in the line
        // handle error while submissions
        if (i < assetNames.size())
        {
            std::cerr << "Invalid input line: missing data for some assets on line " << line << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return assets;
}
