# Charger les bibliothèques nécessaires
# Installe-les avec install.packages("tidyverse") si ce n'est pas déjà fait
library(tidyverse)
library(scales)
library(plotly)


args <- commandArgs(trailingOnly = TRUE)
csv_name <- tools::file_path_sans_ext(basename(args[1]))

# 1. Lecture du fichier CSV
# On précise col_names = FALSE car on va renommer proprement pour éviter les problèmes d'espaces
df <- read_csv(args[1], skip = 1, col_names = c("name", "mean_ns", "stddev_pct", "confidence_pct", "drop")) %>%
  select(-drop) # Supprimer la dernière colonne vide causée par la virgule finale


# 2. Extraction des paramètres depuis la colonne "name"
# IMPORTANT : l'ordre des alternatives compte car "hadi_fwht_batch_f64_contiguous"
# doit être testé AVANT "hadi_fwht_batch_f64" (sinon ce dernier matche en premier
# et tronque le nom de la méthode).
df_parsed <- df %>%
  extract(
    col = name,
    into = c("method", "threads", "nrows", "ncols"),
    regex = "^(hadi_fwht_batch_f64_contiguous|hadi_fwht_batch_f64|fwht_fftw_execute_r2r)_(?:openmp|omp)_(\\d+)_.*\\.dim_(\\d+)x(\\d+)",
    convert = TRUE # Convertit automatiquement les nombres en entiers (integer)
  ) %>%
  mutate(
    mean_s = mean_ns / 1e9,          # conversion nanosecondes -> secondes
    method = factor(method, levels = c(
      "hadi_fwht_batch_f64",
      "hadi_fwht_batch_f64_contiguous",
      "fwht_fftw_execute_r2r"
    )),
    threads = as.factor(threads)
  )

# Affichage de vérification dans la console
print(df_parsed)


# 3. Création du graphique avec ggplot2
fwht_plot <- ggplot(df_parsed, aes(x = ncols, y = mean_s, color = threads, group = threads)) +
  # facettage vertical par methode (lignes), horizontal par nrows (colonnes)
  facet_grid(method ~ nrows, labeller = labeller(nrows = function(x) paste0("nrows = ", x))) +
  geom_line(linewidth = 1) +
  geom_point(size = 3) +

  # Configuration de l'axe X pour afficher les valeurs de ncols telles quelles
  scale_x_continuous(
    breaks = unique(df_parsed$ncols),
    labels = unique(df_parsed$ncols)
  ) +

  labs(
    title = "Impact of memory access pattern on fwht performance",
    subtitle = "Mean execution time across matrix sizes, by method and thread count",
    x = "number of columns",
    y = "mean time (sec)",
    color = "threads"
  ) +
  guides(
    color = guide_legend(nrow = 2)
  ) +
  theme_minimal() +
  theme(
    plot.title       = element_text(face = "bold"),
    legend.position  = "bottom",
    axis.text.x      = element_text(size = 8, angle = 45, hjust = 1),
    strip.text.y     = element_text(angle = 0) # lisibilité des labels de methode
  )


# 4. Affichage et sauvegarde du graphique
#print(fwht_plot)
ggsave(paste0(csv_name, ".pdf"), fwht_plot, device = "pdf", width = 10, height = 9, dpi = 300)
ggsave(paste0(csv_name, ".png"), fwht_plot, width = 10, height = 9, dpi = 300)


# Conversion en plot interactif
interactive_plot <- ggplotly(fwht_plot) %>%
  config(scrollZoom = TRUE)  # Active le zoom à la molette


# Sauvegarde en HTML
htmlwidgets::saveWidget(interactive_plot, paste0(csv_name, ".html"))
