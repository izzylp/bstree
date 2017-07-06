#ifndef _BSTREE_HPP_
#define _BSTREE_HPP_

#include <cstdint>
#include <memory>

template <class T>
class bstree
{
public:
	struct node;
	typedef std::unique_ptr<node> node_ptr;

	struct node {
		T data;
		size_t height;
		node_ptr left;
		node_ptr right;

		node() : height(0), left(nullptr), right(nullptr) {};

		node(const T& data_) : data(data_), height(0), left(nullptr), right(nullptr) {};

		friend class bstree;
	};

	bstree();
	explicit bstree(std::initializer_list<T>);
	void insert(const T&);
	bool find(const T&) const;
	void pre_order(void (*funct)(const T&));
	void in_order(void (*funct)(const T&));
	void pos_order(void (*funct)(const T&));
	void remove(const T&);
	size_t height() const;

private:
	bool find_(const node_ptr&, const T&) const;
	void pre_order(node_ptr&, void (*funct)(const T&));
	void in_order(node_ptr&, void (*funct)(const T&));
	void pos_order(node_ptr&, void (*funct)(const T&));

private:
	/* forbidden operations */
	bstree(bstree&);
	bstree& operator=(bstree&);

private:
	node_ptr root_;
	size_t height_;
};

template <class T>
bstree<T>::bstree()
{
}

template <class T>
bstree<T>::bstree(std::initializer_list<T> init)
{
	for (const T& item : init) {
		insert(item);
	}
}

template <class T> void 
bstree<T>::insert(const T& data)
{
	node_ptr new_node(new node(data));

	if (!root_) {
		root_ = std::move(new_node);
		height_ = 1;
		root_->height = height_;
		return;
	}

	node *tmp = root_.get();
	node *prev = root_.get();

	while (tmp) {
		prev = tmp;
		if (data < tmp->data) {
			tmp = tmp->left.get();
		} else {
			tmp = tmp->right.get();
		}
	}

	size_t new_height = prev->height + 1;
	new_node->height = new_height;
	if (new_height > height_) {
		height_ = new_height;
	}

	if (data < prev->data) {
		prev->left = std::move(new_node);
	} else {
		prev->right = std::move(new_node);
	}
}

template <class T> bool 
bstree<T>::find_(const bstree<T>::node_ptr& node_, const T& data) const
{
	node *tmp = node_.get();

	while (tmp) {
		if (data == tmp->data) {
			break;
		} else if (data < tmp->data) {
			tmp = tmp->left.get();
		} else {
			tmp = tmp->right.get();
		}
	}

	return (tmp != nullptr);
}

template <class T> bool 
bstree<T>::find(const T& item) const
{
	return find_(root_, item);
}

template <class T> void
bstree<T>::pre_order(void (*funct)(const T&))
{
	pre_order(root_, funct);
}

template <class T> void
bstree<T>::in_order(void (*funct)(const T&))
{
	in_order(root_, funct);
}

template <class T> void
bstree<T>::pos_order(void (*funct)(const T&))
{
	pos_order(root_, funct);
}

template <class T> void
bstree<T>::pre_order(node_ptr& node, void (*funct)(const T&))
{
	if (!node) {
		return;
	}

	funct(node->data);
	pre_order(node->left, funct);
	pre_order(node->right, funct);
}

template <class T> void
bstree<T>::in_order(node_ptr& node, void (*funct)(const T&))
{
	if (!node) {
		return;
	}

	in_order(node->left, funct);
	funct(node->data);
	in_order(node->right, funct);
}

template <class T> void
bstree<T>::pos_order(node_ptr& node, void (*funct)(const T&))
{
	if (!node) {
		return;
	}

	pos_order(node->left, funct);
	pos_order(node->right, funct);
	funct(node->data);
}

template <class T> void
bstree<T>::remove(const T& data)
{
	node *tmp = root_.get();
	node *tmp_prev = nullptr;
	node_ptr to_remove;

	while (data != tmp->data) {
		tmp_prev = tmp;
		if (data < tmp->data) {
			tmp = tmp->left.get();
		} else {
			tmp = tmp->right.get();
		}
	}

	if (tmp_prev) {
		if (tmp->data < tmp_prev->data) {
			to_remove = std::move(tmp_prev->left);
		} else {
			to_remove = std::move(tmp_prev->right);
		}
	} else {
		to_remove = std::move(root_);
	}

	if (tmp->left && tmp->right) {
		node *sub = tmp->left.get();
		node *sub_prev = tmp;
		while (sub->right) {
			sub_prev = sub;
			sub = sub->right.get();
		}
		if (!tmp_prev) {
			if (sub_prev != tmp) {
				root_ = std::move(sub_prev->right);
			} else {
				root_ = std::move(sub_prev->left);
			}
		} else {
			if (tmp->data < tmp_prev->data) {
				if (sub_prev != tmp) {
					tmp_prev->left = std::move(sub_prev->right);
				} else {
					tmp_prev->left = std::move(sub_prev->left);
				}
			} else {
				if (sub_prev != tmp) {
					tmp_prev->right = std::move(sub_prev->right);
				} else {
					tmp_prev->right = std::move(sub_prev->left);
				}
			}
		}
		sub->right = std::move(tmp->right);
		if (sub_prev != tmp) {
			sub_prev->right = std::move(sub->left);
			sub->left = std::move(tmp->left);
		}
	} else if (!tmp->left && tmp->right) {
		if (!tmp_prev) {
			root_ = std::move(tmp->right);
		} else {
			if (tmp->data < tmp_prev->data) {
				tmp_prev->left = std::move(tmp->right);
			} else {
				tmp_prev->right = std::move(tmp->right);
			}
		}
	} else if (tmp->left && !tmp->right) {
		if (!tmp_prev) {
			root_ = std::move(tmp->left);
		} else {
			if (tmp->data < tmp_prev->data) {
				tmp_prev->left = std::move(tmp->left);
			} else {
				tmp_prev->right = std::move(tmp->left);
			}
		}
	}
}

template <class T> size_t
bstree<T>::height() const
{
	return height_;
}

#endif
