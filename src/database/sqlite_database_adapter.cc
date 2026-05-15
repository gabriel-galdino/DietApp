#include <memory>
#include <stdexcept>
#include <string>

#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "database/database_statement.h"
#include "database/sqlite_database_adapter.h"
#include "database/sqlite_statement.h"

void SQLiteDatabaseAdapter::Initialize(wxFileName& database) {
  if (database.IsOk() == false) {
    wxStandardPaths& paths = wxStandardPaths::Get();
    database = wxFileName(paths.GetDataDir(), "");
    database.SetFullName("dietapp.db");
  }
  database_.Open(database.GetFullPath());

  if (database_.IsOpen() == false) {
    throw std::runtime_error("Falha ao abrir banco: " +
                             database.GetFullPath().utf8_string());
  }

  if (database_.IsForeignKeySupportEnabled() == false) {
    database_.EnableForeignKeySupport(true);
  }

  wxString sql;
  wxFile sql_file("../schema-model/schema.sql");
  sql_file.ReadAll(&sql);
  database_.ExecuteUpdate(sql);
}

void SQLiteDatabaseAdapter::Shutdown() {
  database_.Close();
}

std::unique_ptr<DatabaseStatement> SQLiteDatabaseAdapter::Prepare(
    const std::string& sql) {
  return std::make_unique<SQLiteStatement>(database_.PrepareStatement(sql));
}

long long SQLiteDatabaseAdapter::GetLastInsertId() {
  return database_.GetLastRowId().GetValue();
}

void SQLiteDatabaseAdapter::BeginTransaction() {
  database_.Begin();
}

void SQLiteDatabaseAdapter::CommitTransaction() {
  database_.Commit();
}

void SQLiteDatabaseAdapter::RollbackTransaction() {
  database_.Rollback();
}
