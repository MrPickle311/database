#pragma once
#include <string>
#include <vector>
#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_set.h>
#include <tbb/concurrent_queue.h>
#include <set>
#include <fstream>
#include <sstream>
#include <iostream>

namespace db
{

    class GlobalRepository;
    class DataExporter;
    class DataImporter;

    /**
     * \class KeysStorage
     * \brief Class for storing and managing all keys registered in the database.
     *
     * This class provides thread-safe methods for adding, checking existence, removing, and retrieving all stored keys.
     */
    class KeysStorage
    {
    private:
        tbb::concurrent_set<std::string> keys_;

    public:
        /**
         * Adds a key to the storage.
         *
         * This method uses a thread-safe concurrent set to insert the provided key.
         *
         * @param key The key to be added.
         */
        void add(const std::string &key)
        {
            keys_.insert(key);
        }

        /**
         * Checks if a key exists in the storage.
         *
         * This method uses the concurrent set's find operation to check if the key exists.
         *
         * @param key The key to be checked.
         * @return True if the key exists, false otherwise.
         */
        bool contains(const std::string &key)
        {
            return keys_.find(key) != keys_.end();
        }

        /**
         * Removes a key from the storage.
         *
         * This method uses the concurrent set's unsafe_erase operation to remove the specified key.
         * **Note:** unsafe_erase should be used with caution in multi-threaded environments. Ensure proper synchronization if necessary.
         *
         * @param key The key to be removed.
         */
        void remove(const std::string &key)
        {
            keys_.unsafe_erase(key);
        }

        std::set<std::string> get_keys()
        {
            return std::set<std::string>(keys_.begin(), keys_.end());
        }

        /**
         * Singleton access method returning a reference to the single instance of KeysStorage.
         *
         * This method ensures there is only one instance of the KeysStorage class throughout the program.
         * @return A reference to the single instance of KeysStorage.
         */
        static KeysStorage &get_instance()
        {
            static KeysStorage instance;
            return instance;
        }
    };

    /**
     * \class StringRepository
     * \brief A class providing thread-safe storage and manipulation of string values.
     *
     * The class offers various methods for managing and modifying string data identified by unique names.
     */
    class StringRepository
    {
        friend class GlobalRepository;
        friend class DataExporter;
        friend class DataImporter;

    public:
        /**
         * Creates a new string with the given name and value.
         *
         * @param name The name of the string.
         * @param value The initial value of the string.
         */
        void create(const std::string &name, const std::string &value);

        /**
         * Returns the value of the string with the given name.
         *
         * @param name The name of the string.
         * @return The value of the string, or an empty string if the string does not exist.
         */
        std::string get(const std::string &name);

        /**
         * Checks if a string with the given name exists.
         *
         * @param name The name of the string.
         * @return True if a string with the given name exists, false otherwise.
         */
        bool exists(const std::string &name);

        /**
         * Returns the length of the string with the given name.
         *
         * @param name The name of the string.
         * @return The length of the string, or 0 if the string does not exist.
         */
        unsigned int length(const std::string &name);

        /**
         * Returns a substring of the string with the given name.
         *
         * @param name The name of the string.
         * @param start The index of the first character in the substring.
         * @param end The index of the last character in the substring.
         * @return The substring, or an exception is thrown if the string does not exist or the specified range is out of bounds.
         */
        std::string substring(const std::string &name, const unsigned int start, const unsigned int end);

        /**
         * Appends the given string to the end of the string with the given name.
         *
         * @param name The name of the string.
         * @param postifx The string to be appended.
         */
        void append(const std::string &name, const std::string &postifx);

        /**
         * Prepends the given string to the beginning of the string with the given name.
         *
         * @param name The name of the string.
         * @param preifx The string to be prepended.
         */
        void prepend(const std::string &name, const std::string &preifx);

        /**
         * Inserts the given string at the specified index in the string with the given name.
         *
         * @param name The name of the string.
         * @param value The string to be inserted.
         * @param index The index at which to insert the string.
         */
        void insert(const std::string &name, const std::string &value, unsigned int index);

        /**
         * Trims the string with the given name by removing characters from the beginning or end.
         *
         * @param name The name of the string.
         * @param start The index of the first character to be removed.
         * @param end The index of the last character to be removed.
         */
        void trim(const std::string &name, const unsigned int start, const unsigned int end);

        /**
         * Trims the string with the given name by removing characters from the beginning.
         *
         * @param name The name of the string.
         * @param count The number of characters to be removed from the beginning.
         */
        void ltrim(const std::string &name, const unsigned int count);

        /**
         * Trims the string with the given name by removing characters from the end.
         *
         * @param name The name of the string.
         * @param count The number of characters to be removed from the end.
         */
        void rtrim(const std::string &name, const unsigned int count);

        static StringRepository &get_instance()
        {
            static StringRepository instance;
            return instance;
        }

    private:
        tbb::concurrent_hash_map<std::string, std::string> data_;
    };

    /**
     * \class SetRepository
     * \brief A class providing thread-safe storage and manipulation of sets of strings.
     *
     * The class offers methods for managing and modifying sets identified by unique names. Each set can contain unique string values.
     */
    class SetRepository
    {
        friend class GlobalRepository;
        friend class DataExporter;
        friend class DataImporter;

    private:
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>> data_;

    public:
        /**
         * Creates a new empty set with the given name.
         *
         * \param name The name of the set to be created.
         */
        void create(const std::string &name);

        /**
         * Adds a new string value to the set identified by the given name.
         *
         * \param name The name of the set.
         * \param value The string value to be added to the set.
         */
        void add(const std::string &name, const std::string &value);

        /**
         * Returns the number of elements in the set identified by the given name.
         *
         * \param name The name of the set.
         * \return The number of elements (strings) in the set.
         */
        unsigned int len(const std::string &name);

        /**
         * Calculates the intersection of elements from multiple sets identified by names in the provided vector.
         * The intersection includes elements present in all specified sets.
         *
         * \param names A vector containing names of the sets to be used for intersection.
         * \return A vector containing the elements that are present in all the sets.
         */
        std::vector<std::string> intersection(const std::vector<std::string> &names);

        /**
         * Calculates the difference between two sets identified by the provided names.
         * The difference includes elements present in the first set but not in the second.
         *
         * \param name_1 The name of the first set.
         * \param name_2 The name of the second set.
         * \return A vector containing the elements that are present in the first set but not in the second.
         */
        std::vector<std::string> difference(const std::string &name_1, const std::string &name_2);

        /**
         * Calculates the union of elements from multiple sets identified by names in the provided vector.
         * The union includes all unique elements present in any of the specified sets.
         *
         * \param names A vector containing names of the sets to be used for union.
         * \return A vector containing the elements that are present in any of the sets.
         */
        std::vector<std::string> union_(const std::vector<std::string> &names); // underscore used to avoid conflict with C++ union keyword

        /**
         * Checks if a specific string value exists within the set identified by the given name.
         *
         * \param name The name of the set.
         * \param value The string value to be searched for.
         * \return True if the value exists in the set, false otherwise.
         */
        bool contains(const std::string &name, const std::string &value);

        /**
         * Retrieves all string values from the set identified by the given name.
         *
         * \param name The name of the set.
         * \return A vector containing all string elements in the set, or an empty vector if the set does not exist.
         */
        std::vector<std::string> get_all(const std::string &name);

        /**
         * Removes and returns a single string value from the set identified by the given name.
         * If the value does not exist or the set is empty, an empty string is returned.
         *
         * \param name The name of the set.
         * \param value The string value to be removed.
         * \return The removed string value. If the set does not exists or the value does not exist then exception is thrown.
         */
        std::string pop(const std::string &name, const std::string &value);

        /**
         * Singleton access method returning a reference to the single instance of SetRepository.
         *
         * @return A reference to the single instance of SetRepository.
         */
        static SetRepository &get_instance()
        {
            static SetRepository instance;
            return instance;
        }
    };

    /**
     * \class QueueRepository
     * \brief A class providing thread-safe storage and manipulation of queues of strings.
     *
     * The class offers methods for managing and modifying queues identified by unique names. Each queue follows the First-In-First-Out (FIFO) principle, where elements are added at the back and removed from the front.
     */
    class QueueRepository
    {
        friend class GlobalRepository;

    private:
        tbb::concurrent_hash_map<std::string, tbb::concurrent_queue<std::string>> data_;

    public:
        /**
         * Creates a new empty queue with the given name.
         *
         * \param name The name of the queue to be created.
         */
        void create(const std::string &name);

        /**
         * Adds a new string value to the back of the queue identified by the given name.
         *
         * \param name The name of the queue.
         * \param value The string value to be added to the queue.
         */
        void push(const std::string &name, const std::string &value);

        /**
         * Removes and returns the front element (the one added first) from the queue identified by the given name.
         * If the queue is empty, an empty string is returned.
         *
         * \param name The name of the queue.
         * \return The string value removed from the front of the queue. If queue is empty, then exception is thrown.
         */
        std::string pop(const std::string &name);

        /**
         * Singleton access method returning a reference to the single instance of QueueRepository.
         *
         * @return A reference to the single instance of QueueRepository.
         */
        static QueueRepository &get_instance()
        {
            static QueueRepository instance;
            return instance;
        }
    };

    /**
     * \class HashRepository
     * \brief A class providing thread-safe storage and manipulation of hashes..
     *
     * The class offers methods for managing and modifying hashes identified by unique names. Each hash  stores key-value pairs of strings.
     */
    class HashRepository
    {
        friend class GlobalRepository;
        friend class DataExporter;
        friend class DataImporter;

    private:
        tbb::concurrent_hash_map<std::string, tbb::concurrent_hash_map<std::string, std::string>> data_;

    public:
        /**
         * Creates a new empty hash  with the given name.
         *
         * \param name The name of the hash  to be created.
         */
        void create(const std::string &name);

        /**
         * Deletes a key-value pair from the hash  identified by the given name.
         *
         * \param name The name of the hash .
         * \param key The key of the key-value pair to be deleted.
         */
        void del(const std::string &name, const std::string &key);

        /**
         * Checks if a specific key exists within the hash  identified by the given name.
         *
         * \param name The name of the hash .
         * \param key The key to be searched for.
         * \return True if the key exists in the hash, false otherwise.
         */
        bool exists(const std::string &name, const std::string &key);

        /**
         * Retrieves the value associated with a specific key from the hash identified by the given name.
         * If the key does not exist, an empty string is returned.
         *
         * \param name The name of the hash.
         * \param key The key of the key-value pair to be retrieved.
         * \return The value associated with the key.
         */
        std::string get(const std::string &name, const std::string &key);

        /**
         * Retrieves all key-value pairs from the hash identified by the given name.
         *
         * \param name The name of the hash .
         * \return A vector containing all key-value pairs (std::pair<std::string, std::string>).
         */
        std::vector<std::pair<std::string, std::string>> get_all(const std::string &name);

        /**
         * Retrieves all keys from the hash  identified by the given name.
         *
         * \param name The name of the hash .
         * \return A vector containing all unique keys (strings) present in the hash , or an empty vector if the hash  does not exist.
         */
        std::vector<std::string> get_keys(const std::string &name);

        /**
         * Sets the value for a specific key in the hash  identified by the given name.
         * If the key already exists, the value is updated. Otherwise, a new key-value pair is created.
         *
         * \param name The name of the hash .
         * \param key The key of the key-value pair
         * \param value The value to be associated with the key.
         */
        void set(const std::string &name, const std::string &key, const std::string &value);

        /**
         * Returns the number of key-value pairs in the hash  identified by the given name.
         *
         * \param name The name of the hash .
         * \return The number of key-value pairs (size) in the hash , or 0 if the hash  does not exist.
         */
        uint len(const std::string &name);

        /**
         * Searches for key-value pairs where the key contains a specific query string.
         *
         * \param name The name of the hash .
         * \param query The query string to search for within values.
         * \return A vector containing key-value pairs (std::pair<std::string, std::string>) where the key contains the query string.
         */
        std::vector<std::string> search(const std::string &name, const std::string &query);

        static HashRepository &get_instance()
        {
            static HashRepository instance;
            return instance;
        }
    };

    /**
     * \class GlobalRepository
     * \brief A central access point for various data repositories.
     *
     * This class provides a single interface to access and manage different types of data repositories. It acts as a facade for underlying repositories like StringRepository, SetRepository, QueueRepository, and HashRepository.
     */
    class GlobalRepository
    {
    private:
        StringRepository &string_repository_;
        SetRepository &set_repository_;
        QueueRepository &queue_repository_;
        HashRepository &hash_repository_;
        KeysStorage &keys_storage_;

        GlobalRepository(StringRepository &string_repository,
                         SetRepository &set_repository,
                         QueueRepository &queue_repository,
                         HashRepository &hash_repository,
                         KeysStorage &keys_storage) : string_repository_(string_repository),
                                                      set_repository_(set_repository),
                                                      queue_repository_(queue_repository),
                                                      hash_repository_(hash_repository),
                                                      keys_storage_{keys_storage} {}

    public:
        /**
         * Retrieves a list of keys matching a specific pattern.
         *
         * It utilizes the KeysStorage to search for keys across different repositories based on a provided pattern (string).
         *
         * \param pattern The pattern (string) to be used for searching keys.
         * \return A vector containing all matching keys (strings) found across different repositories.
         *         Note: An empty vector might be returned if no matches are found.
         */
        std::vector<std::string> keys(std::string &pattern);

        /**
         * Deletes a key from the global storage (if it exists).
         *
         * This method attempts to delete the key from all underlying repositories (StringRepository, SetRepository, etc.) where it might be present.
         *
         * \param key The key (string) to be deleted.
         */
        void del(std::string &key);

        /**
         * Singleton access method returning a reference to the single instance of GlobalRepository.
         *
         * \return A reference to the single instance of GlobalRepository.
         */
        static GlobalRepository &get_instance()
        {
            static GlobalRepository instance{StringRepository::get_instance(), SetRepository::get_instance(), QueueRepository::get_instance(), HashRepository::get_instance(), KeysStorage::get_instance()};
            return instance;
        }
    };

    /**
     * The DataExporter class provides functionality to save data from various repositories (`StringRepository`, `SetRepository`, `HashRepository`) to a file in a specific binary format.
     */
    class DataExporter
    {
    public:
        /**
         * Saves data from all data repositories to a file specified by the `filename` parameter.
         *
         * \param filename The name of the file to save data to.
         * \return True on success, False on failure (e.g., file opening error).
         */
        static bool save(const std::string &filename);

    private:
        /**
         * Saves string data from the `StringRepository` to the file stream.
         *
         * \param file The file stream to save data to.
         */
        static void save_string_data(std::fstream &file);

        /**
         * Saves set data from the `SetRepository` to the file stream.
         *
         * \param file The file stream to save data to.
         */
        static void save_set_data(std::fstream &file);

        /**
         * Saves hash data from the `HashRepository` to the file stream.
         *
         * \param file The file stream to save data to.
         */
        static void save_hash_data(std::fstream &file);
    };

    /**
     * The DataImporter class provides functionality to load data from a file in a specific binary format into various repositories.
     */
    class DataImporter
    {
    public:
        /**
         * Loads data from a file specified by the `filename` parameter and populates the corresponding data repositories (`StringRepository`, `SetRepository`, `HashRepository`).
         *
         * \param filename The name of the file to load data from.
         * \return True on success, False on failure (e.g., file opening error, invalid format).
         */
        static bool load(const std::string &filename);

    private:
        /**
         * Loads string data from the file and inserts it into the `StringRepository`.
         *
         * \param file The input file stream to read data from.
         */
        static void load_string_data(std::ifstream &file);

        /**
         * Loads set data from the file and inserts it into the `SetRepository`.
         *
         * \param file The input file stream to read data from.
         */
        static void load_set_data(std::ifstream &file);

        /**
         * Loads hash data from the file and inserts it into the `HashRepository`.
         *
         * \param file The input file stream to read data from.
         */
        static void load_hash_data(std::ifstream &file);
    };
}