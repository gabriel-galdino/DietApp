CREATE TABLE IF NOT EXISTS "users" (
  id INTEGER PRIMARY KEY,
  name TEXT NOT NULL,
  created_at TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS "foods" (
  `id` INTEGER PRIMARY KEY,
  `name` TEXT NOT NULL,
  `protein_per_100g` REAL NOT NULL,
  `carbs_per_100g` REAL NOT NULL,
  `fats_per_100g` REAL NOT NULL
);
CREATE TABLE IF NOT EXISTS "meals" (
  `id` INTEGER PRIMARY KEY,
  `user_id` INTEGER NOT NULL,
  `name` TEXT NOT NULL,
  `created_at` TEXT NOT NULL,
  FOREIGN KEY(`user_id`) REFERENCES `users`(`id`)
);
CREATE TABLE IF NOT EXISTS "meal_foods" (
  `meal_id` INTEGER NOT NULL,
  `food_id` INTEGER NOT NULL,
  `quantity_grams` REAL NOT NULL,
  `calories` REAL,
  FOREIGN KEY(`meal_id`) REFERENCES `meals`(`id`),
  FOREIGN KEY(`food_id`) REFERENCES `foods`(`id`),
  PRIMARY KEY(`meal_id`, `food_id`)
);