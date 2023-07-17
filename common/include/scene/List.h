/*---------------------------------------------------------------------------*
  Project:  Horizon

  Copyright (C) Nintendo Co., Ltd. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 58466 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_LIST_H_
#define NN_COMMON_SCENE_LIST_H_

namespace scene {

template<class T>
class Node
{
public:
	// Data
	T data;
	// Next node
	Node<T>* pNextNode;
};

template<class T>
class List
{
public:
	// Iterator
	class iterator
	{
	public:
		// Constructor
		iterator(Node<T>* pNode) { m_pNode = pNode; }

		// Return the data reference
		T operator*() { return m_pNode->data; }

		// Compare
		bool operator==(List<T>::iterator it) { return (m_pNode == it.m_pNode); }

		// Compare
		bool operator!=(List<T>::iterator it) { return (m_pNode != it.m_pNode); }

		// Later positioned increment
		void operator++(int) { m_pNode = m_pNode->pNextNode; }

	protected:
		Node<T>* m_pNode;
	};

public:
	// Constructor
	List()
	{
		m_pHeadNode = NULL;
		m_pTailNode = NULL;
	}

	// Add to start
	void push_front(T data)
	{
		Node<T>* pNode = new Node<T>;

		pNode->data		 = data;
		pNode->pNextNode = NULL;

		if (m_pHeadNode == NULL)
		{
			m_pHeadNode = pNode;
		}
		if (m_pTailNode != NULL)
		{
			m_pTailNode->pNextNode = pNode;
		}
		m_pTailNode = pNode;
	}

	// Clear
	void clear()
	{
		Node<T>* pCurNode = m_pHeadNode;

		while (pCurNode)
		{
			Node<T>* pNextNode = pCurNode->pNextNode;

			delete pCurNode;
			pCurNode = pNextNode;
		}

		m_pHeadNode = NULL;
		m_pTailNode = NULL;
	}

	// Return start iterator
	List<T>::iterator begin() { return iterator(m_pHeadNode); }

	// Return terminal iterator
	List<T>::iterator end() { return iterator(NULL); }

protected:
	// Start node
	Node<T>* m_pHeadNode;
	// Terminal node
	Node<T>* m_pTailNode;
};

}  // namespace scene

#endif	// NN_COMMON_SCENE_LIST_H_
