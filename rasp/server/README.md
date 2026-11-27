# README

**Copiar pasta do remoto para o local:**
```bash
rsync -av ecosorter@192.168.0.55://home/ecosorter/server .
```

**Copiar pasta do local para o remoto:**
```bash
rsync -av ./server ecosorter@192.168.0.55://home/ecosorter
```

O serviço systemd é instalado em `~/.config/systemd/user`.
