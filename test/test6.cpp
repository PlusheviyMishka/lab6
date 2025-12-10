#include <gtest/gtest.h>
#include "../include/orc.h"
#include "../include/knight.h"
#include "../include/bear.h"
#include "../include/npc.h"


TEST(OrcTest, OrcCreationAndOutput) {
    Orc orc(10, 20);
    testing::internal::CaptureStdout();
    orc.print();
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_FALSE(output.empty());
}

TEST(KnightTest, KnightKillsOrc) {
    std::shared_ptr<NPC> knight = std::make_shared<Knight>(10, 10);
    std::shared_ptr<NPC> orc = std::make_shared<Orc>(15, 15);

    bool success = orc->accept(knight);
    ASSERT_TRUE(success);
}

TEST(BearTest, BearKillsKnight) {
    std::shared_ptr<NPC> bear = std::make_shared<Bear>(5, 5);
    std::shared_ptr<NPC> knight = std::make_shared<Knight>(5, 8);

    bool success = knight->accept(bear);
    ASSERT_TRUE(success);
}

TEST(KnightTest, KnightOrcAndBear) {
    std::shared_ptr<Orc> orc= std::make_shared<Orc>(40, 50);
    std::shared_ptr<Bear> bear= std::make_shared<Bear>(40, 10);
    std::shared_ptr<Knight> knight= std::make_shared<Knight>(30, 45);

    bool success = knight->fight(orc);
    bool success_b = knight->fight(bear);
    ASSERT_FALSE(success_b);
    ASSERT_TRUE(success);
}

TEST(NPCTest, IsCloseFunction) {
    std::shared_ptr<NPC> orc = std::make_shared<Orc>(0, 0);
    std::shared_ptr<NPC> knight = std::make_shared<Knight>(3, 4);

    ASSERT_TRUE(orc->is_close(knight, 5));
    ASSERT_FALSE(orc->is_close(knight, 4));
}

TEST(NPCTest, MultiNPC) {
    std::shared_ptr<NPC> orc= std::make_shared<Orc>(40, 50);
    std::shared_ptr<NPC> bear= std::make_shared<Bear>(40, 10);
    std::shared_ptr<NPC> knight= std::make_shared<Knight>(30, 45);

    bool success_bear = bear->accept(bear);
    bool success_knight = knight->accept(bear);
    bool success_orc = orc->accept(bear);
    ASSERT_FALSE(success_bear);
    ASSERT_TRUE(success_knight);
    ASSERT_FALSE(success_orc);
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
