#include "query/AlterTableQueryBuilder.hpp"

void AlterTableQueryBuilder::drop(const std::string& column_name) {
    table_ptr_->dropColumn(column_name);
}

void AlterTableQueryBuilder::add(const Column& column) {
    table_ptr_->addColumn(column);
}
