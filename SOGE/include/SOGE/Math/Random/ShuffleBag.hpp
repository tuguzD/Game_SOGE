#ifndef SOGE_MATH_RANDOM_SHUFFLEBAG_HPP
#define SOGE_MATH_RANDOM_SHUFFLEBAG_HPP

#include "SOGE/Math/Random/Random.hpp"
#include "SOGE/Core/Timestep.hpp"

#include <EASTL/vector.h>
#include <stdexcept>


namespace soge
{
    /**
     * @class ShuffleBag ShuffleBag.hpp "SOGE/Math/ShuffleBag.hpp"
     * @brief A utility class that ensures random selection of items with uniform distribution.
     * @details The ShuffleBag stores a collection of items. Each item can be randomly drawn
     * without replacement until the bag is emptied. Once all items have been drawn the bag
     * reshuffles, ensuring fair distribution.
     * @tparam ItemType The type of items stored in the shuffle bag.
     */
    template<typename ItemType>
    class ShuffleBag
    {
    private:
        eastl::vector<ItemType> m_bagItems;
        eastl::vector<ItemType> m_shuffledBag;
        int m_currentIndex;

        void DoShuffle();

    public:
        ShuffleBag();
        ~ShuffleBag();

        /**
         * @brief Adds a specified item to the bag with a given count.
         * @param[in] aItem - The item to add.
         * @param[in] aCount -  The number of times the item should be added to the bag.
         */
        void AddItem(const ItemType& aItem, int aCount = 1);

        /**
         * @brief Adds an item to the bag with a weight that determines how often it appears.
         * @param[in] aItem - The item to add.
         * @param[in] aWeight - How often item should appears.
         */
        void AddItemWeighted(const ItemType& aItem, float aWeight);

        /**
         * @brief  Replaces all occurrences of one item with another in the bag.
         * @param[in] aOldItem - The old item wanted to replace.
         * @param[in] aNewItem - New item to replace old one with.
         */
        void ReplaceItem(const ItemType& aOldItem, const ItemType& aNewItem);

        /**
        * @brief Resets the bag, reshuffling all items.
        */
        void Reshuffle();

        /**
        * @brief Removes all instances of a specific item from the bag..
        * @param[in] aItem - The item which instances needs to remove.
        */
        void RemoveItem(const ItemType& aItem);

        /**
        * @brief Removes all items from the bag and resets the internal state.
        */
        void Clear();

        /**
        * @brief Draws a random item from the bag.
        * @returns A randomly selected item from the bag.
        * @throws std::runtime_error If the bag is empty.
        */
        ItemType Draw();

        /**
        * @brief Returns the next item in the shuffled bag without removing it.
        * @returns The next item in the shuffled bag.
        * @throws std::runtime_error If the bag is empty.
        */
        ItemType Peek() const;

        /**
         * @brief Returns a copy of all the items in the bag (not the shuffled state).
         * @returns A copy of all the items in the bag (not the shuffled state).
         */
        eastl::vector<ItemType> GetItems() const;

        /**
        * @brief Checks if the bag is empty.
        * @returns True if the bag is empty. Otherwise, false.
        */
        bool IsEmpty() const;

        /**
        * @brief Checks whether a specific item exists in the bag.
        * @returns True if item exists in the bag. Otherwise, false.
        * @param[in] aItem - Item which existence need to be checked.
        */
        bool Contains(const ItemType& aItem) const;

        /**
         * @brief Gets the total number of items currently in the ShuffleBag.
         * @returns The number of items in the bag.
         */
        int Count() const;

        /**
         * @brief Gets the total number of curtain item instances currently in the shuffle bag.
         * @returns The number of given item instances in shuffle bag.
         * @param[in] aItem - Item which instances needs to be count.
         */
        int CountItem(const ItemType& aItem) const;

        /**
         * @brief Returns the number of items left in the current shuffled bag.
         * @returns The number of items remaining to be drawn before reshuffling.
         */
        int Remaining() const;

    };

    /////////////////////
    // Implementation
    /////////////////////

    template <typename ItemType>
    inline ShuffleBag<ItemType>::ShuffleBag() : m_currentIndex(0)
    {
    }

    template <typename ItemType>
    inline ShuffleBag<ItemType>::~ShuffleBag()
    {
    }

    template <typename ItemType>
    inline eastl::vector<ItemType> ShuffleBag<ItemType>::GetItems() const
    {
        return m_bagItems;
    }

    template <typename ItemType>
    inline void ShuffleBag<ItemType>::DoShuffle()
    {
        Random random(Timestep::GetNanoseconds());
        for (eastl_size_t i = m_shuffledBag.size() - 1; i > 0; --i) // Fisher-Yates Shuffle
        {
            int j = random.GenInt(0, static_cast<int>(i));
            eastl::swap(m_shuffledBag[i], m_shuffledBag[j]);
        }
    }

    template <typename ItemType>
    inline void ShuffleBag<ItemType>::AddItem(const ItemType& aItem, int aCount)
    {
        for (int i = 0; i < aCount; i++)
            m_bagItems.push_back(aItem);
        Reshuffle();
    }

    template <typename ItemType>
    inline ItemType ShuffleBag<ItemType>::Draw()
    {
        // Probably not the best thing to made... But will fork for now.
        if (m_shuffledBag.empty())
            throw std::runtime_error("Shuffle bag was empty");
        if (m_currentIndex >= static_cast<int>(m_shuffledBag.size()))
            Reshuffle();
        return m_shuffledBag[m_currentIndex++];
    }

    template <typename ItemType>
    inline ItemType ShuffleBag<ItemType>::Peek() const
    {
        if (m_currentIndex + 1 >= static_cast<int>(m_shuffledBag.size()))
            throw std::runtime_error("Shuffle bag was empty or needs to reshuffle");
        return m_shuffledBag[m_currentIndex++];
    }

    template <typename ItemType>
    inline void ShuffleBag<ItemType>::AddItemWeighted(const ItemType& aItem, float aWeight)
    {
        int count = static_cast<int>(aWeight * 10); // Scale weight to integer count
        AddItem(aItem, count);
    }

    template <typename ItemType>
    inline void ShuffleBag<ItemType>::ReplaceItem(const ItemType& aOldItem, const ItemType& aNewItem)
    {
        eastl::replace(m_bagItems.begin(), m_bagItems.end(), aOldItem, aNewItem);
        Reshuffle();
    }

    template <typename ItemType>
    inline void ShuffleBag<ItemType>::Reshuffle()
    {
        m_shuffledBag = m_bagItems; // Copy all original items into shuffled bag
        DoShuffle();
        m_currentIndex = 0;
    }

    template <typename ItemType>
    inline void ShuffleBag<ItemType>::RemoveItem(const ItemType& aItem)
    {
        m_bagItems.erase(eastl::remove(m_bagItems.begin(), m_bagItems.end(), aItem), m_bagItems.end());
        Reshuffle();
    }

    template <typename ItemType>
    inline void ShuffleBag<ItemType>::Clear()
    {
        m_bagItems.clear();
        m_shuffledBag.clear();
        m_currentIndex = 0;
    }

    template <typename ItemType>
    inline bool ShuffleBag<ItemType>::IsEmpty() const
    {
        return m_bagItems.empty();
    }

    template <typename ItemType>
    inline bool ShuffleBag<ItemType>::Contains(const ItemType& aItem) const
    {
        return eastl::find(m_bagItems.begin(), m_bagItems.end(), aItem) != m_bagItems.end();
    }

    template <typename ItemType>
    inline int ShuffleBag<ItemType>::Count() const
    {
        return static_cast<int>(m_bagItems.size());
    }

    template <typename ItemType>
    inline int ShuffleBag<ItemType>::CountItem(const ItemType& aItem) const
    {
        return static_cast<int>(eastl::count(m_bagItems.begin(), m_bagItems.end(), aItem));
    }

    template <typename ItemType>
    inline int ShuffleBag<ItemType>::Remaining() const
    {
        return static_cast<int>(m_shuffledBag.size()) - m_currentIndex;
    }
}

#endif // SOGE_MATH_RANDOM_SHUFFLEBAG_HPP
