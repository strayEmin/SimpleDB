#pragma once

#include <gtest/gtest.h>

#include "../include/utils/TypeClassifier.hpp"

void test_TypeClassifier() {
    assert(TypeClassifier::specificToGeneralType("VARCHAR") ==
           TypeClassifier::GeneralType::TEXT);
    assert(TypeClassifier::specificToGeneralType("CHAR") ==
           TypeClassifier::GeneralType::TEXT);
    assert(TypeClassifier::specificToGeneralType("TEXT") ==
           TypeClassifier::GeneralType::TEXT);
    assert(TypeClassifier::specificToGeneralType("CLOB") ==
           TypeClassifier::GeneralType::TEXT);

    assert(TypeClassifier::specificToGeneralType("INT") ==
           TypeClassifier::GeneralType::NUMERIC);
    assert(TypeClassifier::specificToGeneralType("INTEGER") ==
           TypeClassifier::GeneralType::NUMERIC);
    assert(TypeClassifier::specificToGeneralType("BIGINT") ==
           TypeClassifier::GeneralType::NUMERIC);
    assert(TypeClassifier::specificToGeneralType("SMALLINT") ==
           TypeClassifier::GeneralType::NUMERIC);
    assert(TypeClassifier::specificToGeneralType("DECIMAL") ==
           TypeClassifier::GeneralType::NUMERIC);
    assert(TypeClassifier::specificToGeneralType("NUMERIC") ==
           TypeClassifier::GeneralType::NUMERIC);
    assert(TypeClassifier::specificToGeneralType("FLOAT") ==
           TypeClassifier::GeneralType::NUMERIC);
    assert(TypeClassifier::specificToGeneralType("DOUBLE") ==
           TypeClassifier::GeneralType::NUMERIC);
    assert(TypeClassifier::specificToGeneralType("REAL") ==
           TypeClassifier::GeneralType::NUMERIC);

    assert(TypeClassifier::specificToGeneralType("DATE") ==
           TypeClassifier::GeneralType::DATE_OR_TIME);
    assert(TypeClassifier::specificToGeneralType("TIME") ==
           TypeClassifier::GeneralType::DATE_OR_TIME);
    assert(TypeClassifier::specificToGeneralType("TIMESTAMP") ==
           TypeClassifier::GeneralType::DATE_OR_TIME);
    assert(TypeClassifier::specificToGeneralType("DATETIME") ==
           TypeClassifier::GeneralType::DATE_OR_TIME);

    assert(TypeClassifier::parseEnteredValueType("\"hello\"") ==
           TypeClassifier::GeneralType::TEXT);
    assert(TypeClassifier::parseEnteredValueType("\"123abc\"") ==
           TypeClassifier::GeneralType::TEXT);
    assert(TypeClassifier::parseEnteredValueType("") ==
           TypeClassifier::GeneralType::TEXT);

    assert(TypeClassifier::parseEnteredValueType("123") ==
           TypeClassifier::GeneralType::NUMERIC);
    assert(TypeClassifier::parseEnteredValueType("3.14") ==
           TypeClassifier::GeneralType::NUMERIC);
    assert(TypeClassifier::parseEnteredValueType("-15") ==
           TypeClassifier::GeneralType::NUMERIC);
    assert(TypeClassifier::parseEnteredValueType("0") ==
           TypeClassifier::GeneralType::NUMERIC);
    assert(TypeClassifier::parseEnteredValueType("+42") ==
           TypeClassifier::GeneralType::NUMERIC);

    assert(TypeClassifier::parseEnteredValueType("\"2023-01-15\"") ==
           TypeClassifier::GeneralType::DATE_OR_TIME);
    assert(TypeClassifier::parseEnteredValueType("\"12:30:45\"") ==
           TypeClassifier::GeneralType::DATE_OR_TIME);

    assert(TypeClassifier::specificToGeneralType("varchar(255)") ==
           TypeClassifier::GeneralType::TEXT);
    assert(TypeClassifier::specificToGeneralType("VarChar") ==
           TypeClassifier::GeneralType::TEXT);

    assert(TypeClassifier::parseEnteredValueType("\"2023-01-15\"") ==
           TypeClassifier::GeneralType::DATE_OR_TIME);
    assert(TypeClassifier::parseEnteredValueType("\"12:30:45\"") ==
           TypeClassifier::GeneralType::DATE_OR_TIME);

    assert(TypeClassifier::parseEnteredValueType("") ==
           TypeClassifier::GeneralType::TEXT);
}