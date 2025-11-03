#define BOOST_TEST_MODULE test_version

#include <boost/test/unit_test.hpp>

#include "libs/async.hpp"

BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(Test) {
  async::Context* ctx = async::connect(3);

  async::receive(ctx, "cmd1");
  async::receive(ctx, "cmd2");
  async::receive(ctx, "cmd3");
  async::receive(ctx, "cmd4");
  async::receive(ctx, "cmd5");

  async::receive(ctx, "{");
  async::receive(ctx, "cmd6");
  async::receive(ctx, "cmd7");
  async::receive(ctx, "cmd8");
  async::receive(ctx, "}");

  async::disconnect(ctx);
}

BOOST_AUTO_TEST_CASE(TestContext) {
  async::Context* ctx1 = async::connect(2);
  async::receive(ctx1, "ctx1_cmd1");
  async::receive(ctx1, "ctx1_cmd2");

  async::Context* ctx2 = async::connect(4);
  async::receive(ctx2, "ctx2_cmd1");
  async::receive(ctx2, "ctx2_cmd2");
  async::receive(ctx2, "ctx2_cmd3");
  async::receive(ctx2, "ctx2_cmd4");

  async::disconnect(ctx1);
  async::disconnect(ctx2);
}

BOOST_AUTO_TEST_SUITE_END()
