#ifndef META_ASSERT_HPP__
#define META_ASSERT_HPP__

// Assert {{
template<typename...Preds>
struct Assert {
  static_assert((typename Preds::Type() && ...));
};
// }} Assert

#endif
