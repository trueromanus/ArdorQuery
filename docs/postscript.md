# Console Application

To download binary build please check the [github repository](https://github.com/EmptyFlow/PostgreSQL.Migrations/releases).

## Commands
 
* `apply [options]` - Apply all new migrations to database(s)
* `revert [options]` - Revert database to state before migration specified in parameter
* `force-revert [options]` - Revert only one migration specified in parameter
* `version` - Display version information

### apply
For check available options use command `apply --help`.    

* `-f [file] [file] or --files [file] [file]` - [required] List of files containing migrations.
* `-c [string] [string] or --connectionStrings [string] [string]` - [required] List of connection strings to which migrations will be applied.
* `-s [string] or --strategy [string]` - [default = MigrationResolverAttribute] Select strategy for read migrations.
* `-g  [string] or --group [string]` - If you specify some group or groups (separated by commas), migrations will be filtered by these groups

### revert
For check available options use command `revert --help`.    

* `-m [number] or --migration [number]` - [required] The parameter specifies the number of the migration to which you want to roll back the changes
* `-f [file] [file] or --files [file] [file]` - [required] List of files containing migrations.
* `-c [string] [string] or --connectionStrings [string] [string]` - [required] List of connection strings to which migrations will be applied
* `-s [string] or --strategy [string]` - [default = MigrationResolverAttribute] Select strategy for read migrations.
* `-g  [string] or --group [string]` - If you specify some group or groups (separated by commas), migrations will be filtered by these groups

### force-revert
For check available options use command `force-revert --help`.    

* `-m [number] or --migration [number]` - [required] The parameter specifies the number of the migration which will be reverted (if it was applied before) and after it applied once again
* `-f [file] [file] or --files [file] [file]` - [required] List of files containing migrations.
* `-c [string] [string] or --connectionStrings [string] [string]` - [required] List of connection strings to which migrations will be applied.
* `-s [string] or --strategy [string]` - [default = MigrationResolverAttribute] Select strategy for read migrations.
* `-g  [string] or --group [string]` - If you specify some group or groups (separated by commas), migrations will be filtered by these groups

## Strategies

### MigrationResolverAttribute
Migrations are organized into C# classes.
Each class inherits from the `MigrationScript` class from the `PostgreSQL.Migrations` assembly and decorated `MigrationNumber` attribute.
You must implement the `Up` and `Down` methods, where `Up` returns the SQL script that will be executed during the `Apply operation`, and `Down` returns the SQL script that will be executed during the `Revert operation`.
Optional you can fill fields `Issue` (to bound the issue from bugtracker) and `Group` (to bound migration with group or groups).
```csharp
[MigrationNumber ( 1, "http://issue/1", "firstGroup" )]
public class InitialMigration : MigrationScript {

    public override string Down () => "DROP TABLE test;";

    public override string Up () => "CREATE TABLE test(id int4);";

}
```