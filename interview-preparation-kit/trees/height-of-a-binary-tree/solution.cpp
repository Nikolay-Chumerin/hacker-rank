
/*The tree node has data, left child and right child
class Node {
    int data;
    Node* left;
    Node* right;
};

*/
int height(Node* root) {
  if (!root) return 0;
  int l{0}, r{0};
  if (root->left) l = 1 + height(root->left);
  if (root->right) r = 1 + height(root->right);
  return max(l, r);
}
