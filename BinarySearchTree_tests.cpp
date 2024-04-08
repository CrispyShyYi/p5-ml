#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"
#include <sstream>

using namespace std;

TEST(test_empty_impl) {
    BinarySearchTree<int> tree;
    
    ASSERT_TRUE(tree.empty());

    tree.insert(5); // 5
    ASSERT_FALSE(tree.empty());
}

TEST(size_impl){
    BinarySearchTree<int> tree;

    ASSERT_EQUAL(tree.size(), 0);

    tree.insert(3); 
    // 3
    tree.insert(4); 
    // 3
    //    4
    ASSERT_EQUAL(tree.size(), 2);

    tree.insert(1); 
    //    3
    //  1   4
    ASSERT_EQUAL(tree.size(), 3);
}

TEST(height_impl){
    BinarySearchTree<int> tree;

    ASSERT_EQUAL(tree.height(), 0);

    tree.insert(3); 
    // 3
    tree.insert(4); 
    // 3\
    //   4
    ASSERT_EQUAL(tree.height(), 2);

    tree.insert(1); 
    //   /3\
    //  1   4
    ASSERT_EQUAL(tree.height(), 2);

    tree.insert(8);
    tree.insert(10);
    //  /3\
    // 1   4\
    //       9\
    //         10
    ASSERT_EQUAL(tree.height(), 4);

    tree.insert(7);
    tree.insert(6);
    //  /3\
    // 1   4\
    //       /8\
    //     /7   10
    //    6
    ASSERT_EQUAL(tree.height(), 5);
}

TEST(copy_nodes_impl){
    BinarySearchTree<int> tree;

    tree.insert(3);          
    tree.insert(4); 
    tree.insert(1); 
    tree.insert(8);
    tree.insert(10);
    tree.insert(7);
    tree.insert(6);
    //  /3\
    // 1   4\
    //       /8\
    //     /7   10
    //    6

    BinarySearchTree<int> tree_copy1(tree);
    BinarySearchTree<int>::Iterator it = tree.begin();
    BinarySearchTree<int>::Iterator it_copy;
    
    ASSERT_TRUE(it_copy == tree.end());
    it_copy = tree.begin(); // 1;
    ASSERT_EQUAL(*it, 1);

    ++it; // 3;
    ASSERT_EQUAL(*it, 3);
    ASSERT_TRUE(tree.find(3) == it);

    tree.insert(2);
    //  /3\
    // 1\   4\
    //   2    /8\
    //      /7   10
    //     6
    it = tree.begin(); // 1;
    ++it; // 2;
    ASSERT_EQUAL(*it, 2);
    ++it; // 3;
    ASSERT_EQUAL(*it, 3);
    ASSERT_EQUAL(*it++, 3);
    ASSERT_EQUAL(*it, 4);
    it_copy = it++;
    ASSERT_TRUE(it_copy != it); // &it_copy != &it;
    ASSERT_EQUAL(*it_copy, 4);
    ASSERT_EQUAL(*it, 6);
    ++(++it); // 8;
    ++(++it); // nullptr;
    ASSERT_TRUE(it == tree.end()); // &it == nullptr;
    ASSERT_TRUE(tree.find(12) == tree.end());
    ASSERT_TRUE(tree.find(1) == tree.begin());
}

TEST(destroy_nodes_impl){
    BinarySearchTree<int> tree;
    tree.insert(3);          
    tree.insert(4); 
    tree.insert(1); 
    tree.insert(8);
    tree.insert(10);
    tree.insert(7);
    tree.insert(6);
    //  /3\
    // 1   4\
    //       /8\
    //     /7   10
    //    6

    BinarySearchTree<int> tree_copy;
    tree_copy.insert(3);
    tree_copy.insert(2);
    tree_copy.insert(9);
    tree_copy.insert(4);
    //    /3\
    //   2  /9
    //     4
    tree_copy = tree;
    BinarySearchTree<int>::Iterator it = tree_copy.begin();
    ASSERT_TRUE(*it == 1);
    ++(++it); // 4;
    ASSERT_TRUE(*it == 4);
    ++it; // 6;
    ASSERT_TRUE(*it == 6);
    ASSERT_TRUE(tree_copy.find(9) == tree_copy.end());
}

TEST(min_max_element_impl){
    BinarySearchTree<int> tree;
    tree.insert(3);          
    tree.insert(4);  
    tree.insert(8);
    tree.insert(10);
    tree.insert(7);
    tree.insert(6);
    //   3\
    //     4\
    //       /8\
    //     /7   10
    //    6

    ASSERT_EQUAL(*tree.min_element(), 3);
    ASSERT_EQUAL(*tree.min_element(), *tree.begin());

    BinarySearchTree<int>::Iterator it = tree.find(10);
    ASSERT_EQUAL(*it, *tree.max_element());
    ASSERT_TRUE(it != tree.end());
}

TEST(check_sorting_invariant_impl){
    BinarySearchTree<int> tree;
    tree.insert(3); 
    tree.insert(1);        
    tree.insert(4);  
    tree.insert(8);
    tree.insert(10);
    tree.insert(7);
    tree.insert(6);
    //   /3\
    //  1   4\
    //       /8\
    //     /7   10
    //    6

    BinarySearchTree<int>::Iterator it = tree.begin(); // 1;
    it = tree.find(7);
    ASSERT_TRUE(tree.check_sorting_invariant());
    *it = 2;
    //   /3\
    //  1   4\
    //       /8\
    //     /2   10
    //    6
    ASSERT_FALSE(tree.check_sorting_invariant());

    BinarySearchTree<int> tree_alt;
    tree_alt.insert(4);
    tree_alt.insert(2);
    tree_alt.insert(7);
    tree_alt.insert(1);
    tree_alt.insert(3);
    /*        /4\
            /2\   7
           1   3
    */
   it = tree_alt.find(3);
   *it = 5;
   /*        /4\
            /2\   7
           1   5
    */
   ASSERT_FALSE(tree_alt.check_sorting_invariant());
}

TEST(traverse_in_pre_order_impl){
    BinarySearchTree<int> tree;
    tree.insert(3); 
    tree.insert(1);        
    tree.insert(4);  
    tree.insert(8);
    tree.insert(10);
    tree.insert(7);
    tree.insert(6);
    //   /3\
    //  1   4\
    //       /8\
    //     /7   10
    //    6

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    cout << oss_inorder.str() << endl << endl;
    ASSERT_TRUE(oss_inorder.str() == "1 3 4 6 7 8 10 ");

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    cout << oss_preorder.str() << endl << endl;
    ASSERT_TRUE(oss_preorder.str() == "3 1 4 8 7 6 10 ");
}

TEST(min_greater_than_impl){
    BinarySearchTree<int> tree;
    tree.insert(3); 
    tree.insert(1);        
    tree.insert(4);  
    tree.insert(8);
    tree.insert(10);
    tree.insert(7);
    tree.insert(6);
    //   /3\
    //  1   4\
    //       /8\
    //     /7   10
    //    6

    ASSERT_TRUE(*tree.min_greater_than(4) == 6);
    ASSERT_TRUE(*tree.min_greater_than(7) == 8);
    ASSERT_TRUE(*tree.min_greater_than(3) == 4);
    tree.insert(2);
    //   /3\
    //  1\  4\
    //    2  /8\
    //     /7   10
    //    6
    ASSERT_TRUE(*tree.min_greater_than(1) == 2);
    ASSERT_TRUE(*tree.min_greater_than(2) == 3);
    ASSERT_TRUE(*tree.min_greater_than(8) == 10);
    ASSERT_TRUE(tree.min_greater_than(10) == tree.end());
}

TEST(operator_arror){
    BinarySearchTree<std::pair<int, double>> tree;
    auto it = tree.insert({3, 4.1});
    tree.insert({1, 5.7});
    tree.insert({4, 5.3});
    tree.insert({2, 2.9});
    tree.insert({8, 0.1});
    tree.insert({7, 9.3});
    tree.insert({6, 2.3});
    tree.insert({10,0.9});
    /*      /3\ 
           1\  4\
             2  /8\ 
              /7   10
             6
    */
   ASSERT_EQUAL(it->first, 3);
   ASSERT_EQUAL(it->second, 4.1);
   ++(++it); // 6;
   ASSERT_EQUAL(it->first, 6);
   ASSERT_EQUAL(it->second, 2.3);
   auto it_copy = it++;
   ASSERT_EQUAL(it_copy->first, 6);
   ASSERT_EQUAL(it_copy->second, 2.3);
   ++it; // 8;
   ASSERT_EQUAL(it->first, 8);
   ASSERT_EQUAL(it->second, 0.1);
}              
TEST_MAIN()
