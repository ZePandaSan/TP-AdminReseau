require 'open3'
require 'colorize'

def ping_machine(address, count)
  output, status = Open3.capture2e("ping -c #{count} #{address}")

  paquets_recus = 0

  if status.success?
    puts "Réponse de #{address}:"
    rtt = 0
    output.scan(/time=(\d+\.\d+)/) do |time|
      rtt += time[0].to_f
      paquets_recus += 1
      puts "Délai de réponse: #{time[0]} ms"
    end
    puts "RTT: #{rtt*2} ms"

    pourcentage_reception = (paquets_recus.to_f / count.to_f) * 100
    puts "Pourcentage de paquets reçus: #{pourcentage_reception}%"

    return rtt*2
  else
    puts "Erreur lors de l'exécution de la commande ping pour l'adresse #{address}."
    puts output
    return nil
  end
end

def ping_machineV2(filename, count=1)
  File.readlines(filename).each do |line|
    address = line.strip
    output, status = Open3.capture2e("ping -c #{count} #{address}")

    paquets_recus = 0
    rtt_total = 0

    output.scan(/time=(\d+\.\d+)/) do |time|
      rtt_total += time[0].to_f
      paquets_recus += 1
    end

    pourcentage_reception = (paquets_recus.to_f / count.to_f) * 100
    rtt_moyen = paquets_recus > 0 ? rtt_total / paquets_recus : "∞"

    puts "#{address} => #{pourcentage_reception.round}% , #{rtt_moyen} ms"
  end
end

def ping_machineV3(filename)
  File.readlines(filename).each do |line|
    address = line.chomp
    output, status = Open3.capture2e("ping -c 1 #{address}")

    if status.success?
      rtt_value = output.scan(/time=(\d+\.\d+)/).first&.first
      if rtt_value
        puts "#{address} => 100, #{rtt_value} ms".green
      else
        puts "#{address} => 100, no time value found".green
      end
    else
      puts "#{address} => 0, +∞ ms".red
    end
  end
end

puts "Entrez l'adresse de la machine distante à pinger:"
adresse = gets.chomp

puts "Entrez le nombre de paquets à envoyer:"
nombre_paquets = gets.chomp.to_i

ping_machine(adresse, nombre_paquets)

puts "Entrez le nom du fichier contenant les adresses IP:"
filename = gets.chomp

puts "Entrez le nombre de paquets à envoyer (facultatif, par défaut = 1):"
nombre_paquets = gets.chomp.to_i
nombre_paquets = 1 if nombre_paquets == 0

ping_machineV2(filename, nombre_paquets)

puts "Meilleur affichage :"

ping_machineV3(filename)




