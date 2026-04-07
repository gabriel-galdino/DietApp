#include <memory>
#include <string>

#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "database/database_statement.h"
#include "database/sqlite_database_adapter.h"
#include "database/sqlite_statement.h"

bool SQLiteDatabaseAdapter::Initialize(wxFileName& database) {
  if (database.IsOk() == false) {
    wxStandardPaths& paths = wxStandardPaths::Get();
    database = wxFileName(paths.GetDataDir(), "");
    database.SetFullName("dietapp.db");
  }
  database_.Open(database.GetFullPath());

  if (database_.IsOpen() == false) {
    return false;
  }

  if (database_.IsForeignKeySupportEnabled() == false) {
    database_.EnableForeignKeySupport(true);
  }

  wxString sql;
  wxFile sql_file("../schema-model/schema.sql");
  sql_file.ReadAll(&sql);
  database_.ExecuteUpdate(sql);
  return true;
}

bool SQLiteDatabaseAdapter::Shutdown() {
  database_.Close();
  return true;
}

std::unique_ptr<DatabaseStatement> SQLiteDatabaseAdapter::Prepare(
    const std::string& sql) {
  return std::make_unique<SQLiteStatement>(database_.PrepareStatement(sql));
}

// bool SQLiteDatabaseAdapter::Execute(const std::string& query) {
//   return false;
// }
//
// std::unique_ptr<QueryResult> SQLiteDatabaseAdapter::Select(
//     const std::string& query) {
//   return nullptr;
// }
//
// std::string SQLiteDatabaseAdapter::GetLastError() {
//   return "";
// }

long long SQLiteDatabaseAdapter::GetLastInsertId() {
  return database_.GetLastRowId().GetValue();
}

bool SQLiteDatabaseAdapter::BeginTransaction() {
  database_.Begin();
  return true;
}

bool SQLiteDatabaseAdapter::CommitTransaction() {
  database_.Commit();
  return true;
}

bool SQLiteDatabaseAdapter::RollbackTransaction() {
  database_.Rollback();
  return true;
}