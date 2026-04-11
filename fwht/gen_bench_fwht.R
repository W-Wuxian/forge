# Charger les bibliothèques nécessaires
# Installe-les avec install.packages("tidyverse") si ce n'est pas déjà fait
library(tidyverse)
library(scales)
library(plotly)

args <- commandArgs(trailingOnly = TRUE)
# 1. Lecture du fichier CSV
# On précise col_names = FALSE car on va renommer proprement pour éviter les problèmes d'espaces
# "output/rotatedata_mat_benchmark.csv"
df <- read_csv(args[1], skip = 1, col_names = c("name", "mean_ns", "stddev_pct", "confidence_pct", "drop")) %>%
  select(-drop) # Supprimer la dernière colonne vide causée par la virgule finale

# 2. Extraction des paramètres depuis la colonne "name"
df_parsed <- df %>%
  extract(
    col = name,
    into = c("method", "nrows", "ncols"),
    regex = "^([^.]+)\\.dim_(\\d+)x(\\d+)",
    convert = TRUE # Convertit automatiquement les nombres en entiers (integer)
  )
#df_parsed <- df_parsed |> filter(method != c("rotatedata_mat_rmaj"))
#df_parsed <- df_parsed |> filter(method != c("rotatedata_rmaj_loop"))
# Affichage de vérification dans la console
print(df_parsed)

# 3. Création du graphique avec ggplot2
fwht_plot <- ggplot(df_parsed, aes(x = ncols, y = mean_ns, color = method, group = method)) +
  facet_grid( ~ nrows + "nrows") +
  geom_line(linewidth = 1) +
  geom_point(size = 3) +
  
  # Configuration de l'axe X pour afficher les puissances de 2 (2^x)
  scale_x_continuous(
    trans = log2_trans(), 
    #breaks = trans_breaks("log2", function(x) 2^x),
    breaks = 2^(10:15),
    labels = trans_format("log2", math_format(2^.x))
  ) +
  
  # Axe Y : Conversion de nanosecondes en secondes
  scale_y_continuous(
    labels = function(y) y / 1e9  # Divise par 1 milliard
  ) +
  # (Optionnel) Axe Y rendu plus lisible
  #scale_y_continuous(labels = scales::label_number(scale_cut = scales::cut_short_scale())) +
  
  labs(
    title = "Impact of memory access pattern on fwht performance",
    subtitle = "Mean execution time across matrix sizes",
    x = expression("number of"~columns), # expression() permet un rendu mathématique du titre
    y = "mean time (sec)",
    color = "function"
  ) +
  guides(
    color = guide_legend(nrow = 4)
  ) +
  theme_minimal() +
  theme(
    plot.title       = element_text(face = "bold"),
    legend.position  = "bottom",
    axis.text.x      = element_text(size = 8, angle = 45, hjust = 1) 
  )

# 4. Affichage et sauvegarde du graphique
#print(fwht_plot)
ggsave("plot_benchmark_fwht.pdf", fwht_plot, device="pdf", width = 8, height = 6, dpi = 300)
ggsave("plot_benchmark_fwht.png", fwht_plot, width = 8, height = 6, dpi = 300)

# Conversion en plot interactif
interactive_plot <- ggplotly(fwht_plot) %>%
  config(scrollZoom = TRUE)  # Active le zoom à la molette

# Sauvegarde en HTML
htmlwidgets::saveWidget(interactive_plot, "fwht_plot_interactive.html")